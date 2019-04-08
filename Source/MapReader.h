#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiProcessor.h"
#include "AppSettings.h"
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
        const StringArray searchTags = StringArray("A", "B", "X", "Y", "DpadUp", "DpadDown", "DpadLeft", 
                                                    "DpadRight", "LStick", "RStick", "RBmpr", "LBmpr", "Start", "Back", "Guide", "RTrigger",
                                                    "LTrigger");

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


        HashMap<String, Array<int>>* getComponentMap(int index) {
            HashMap<String, Array<int>>* componentMap = new HashMap<String, Array<int>>();
            json rawMap = loadedMaps[index];

            for (auto tag : searchTags) {
                if (rawMap["map"][tag.toStdString()].is_string()) {
                    int parsedNote =  GidiProcessor::parseNote(rawMap["map"][tag.toStdString()].get<std::string>());
                    componentMap->set(tag, Array<int>(parsedNote));
                }
                else if (rawMap["map"][tag.toStdString()].is_array()) {
                    componentMap->set(tag, Array<int>());
                    for (auto item : rawMap["map"][tag.toStdString()].get<std::vector<std::string>>()) {
                        componentMap->getReference(tag).add(GidiProcessor::parseNote(item));
                    }
                }
            }
            return componentMap;
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