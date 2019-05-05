#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppSettings.h"
#include "GamepadMap.h"
#include "ComponentType.h"
#include "NoteParser.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/*
    Quick and extremely dirty json map reader
    ~Works~ but needs a lot of work
    Whole program will break if an invalid map is read in
*/

class MapReader {

    private:
        Array<json> loadedMaps;
    

    public:
        // NOTE: These tags should correspond with the ComponentType.h
        const StringArray searchTags = 
            StringArray("A", 
                        "B", 
                        "X", 
                        "Y", 
                        "DpadUp", 
                        "DpadDown", 
                        "DpadLeft", 
                        "DpadRight", 
                        "LBmpr", 
                        "RBmpr", 
                        "LStick", 
                        "RStick", 
                        "Start", 
                        "Back",
                        "Guide", 
                        "LTrigger",
                        "RTrigger",
                        "LStickX",
                        "LStickY",
                        "RStickX",
                        "RStickY" 
                       );

        MapReader() {
            refresh();
        }

        ~MapReader() {}

        void refresh() {
            File dir(AppSettings::getMapDirectory());
            loadedMaps.clear();
            if (dir.exists() && dir.isDirectory()) {
                Array<File> maps = dir.findChildFiles(File::TypesOfFileToFind::findFiles, true, "*.json");
                for (auto map : maps) {
                    loadedMaps.add(json::parse(map.loadFileAsString().toStdString()));
                }
            }
            else {
                printf("Couldn't find mappings directory...\n");
            }
        }

        Array<json> getLoadedMaps() {
            return loadedMaps;
        }

        StringArray getLoadedMapNames() {
            StringArray names;
            for (auto map : loadedMaps) {
                names.add(map["info"]["name"].get<std::string>());
            }
            return names;
        }

        GamepadMap<Array<int>>* createComponentMap(int index) 
        {
            GamepadMap<Array<int>>* compMap = new GamepadMap<Array<int>>();
            
            json rawMap = loadedMaps[index];

            for (auto tag : searchTags) {
                Array<int> mappedFunctions;
                if (rawMap["map"][tag.toStdString()].is_string()) { // component only holds 1 function
                    mappedFunctions.add(parseNote(rawMap["map"][tag.toStdString()].get<std::string>()));
                }
                else if (rawMap["map"][tag.toStdString()].is_array()) {
                    for (auto item : rawMap["map"][tag.toStdString()].get<std::vector<std::string>>()) {
                        mappedFunctions.add(parseNote(item));
                    }
                }

                // Now we have our mapped functions, we can add them to our component map
                if (mappedFunctions.size() > 0) {
                    ComponentType ct = (ComponentType) searchTags.indexOf(tag);
                    compMap->insert_or_assign(ct, mappedFunctions);
                }
            }
            return compMap;
        }

        struct MapInfo {
            String name;
            String author;
        };

        MapInfo getMapInfo(int index) {
            MapInfo info;
            json rawMap = loadedMaps[index];
            if (rawMap["info"]["name"].is_string()) {
                info.name = rawMap["info"]["name"].get<std::string>();
            }
            if (rawMap["info"]["author"].is_string()) {
                info.author = rawMap["info"]["author"].get<std::string>();
            }
            return info;
        }
};