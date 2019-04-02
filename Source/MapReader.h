#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiProcessor.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class MapReader {

    private:
        const String PATH2 = "/home/mason/dev/mdGidi/Resources/mappings";
        Array<json> loadedMaps;
    

    public:
        const StringArray searchTags = StringArray("A", "B", "X", "Y", "DpadUp", "DpadDown", "DpadLeft", 
                                                    "DpadRight", "LStick", "RStick", "RBmpr", "LBmpr", "Start", "Back", "Guide");

        MapReader() {
            File dir(PATH2);
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

        ~MapReader() {}

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


        HashMap<String, int>* getComponentMap(int index) {
            HashMap<String, int> *componentMap = new HashMap<String, int>();
            json rawMap = loadedMaps[index];

            for (auto tag : searchTags) {
                if (rawMap["map"][tag.toStdString()].is_string()) {
                    componentMap->set(tag, GidiProcessor::parseNote(rawMap["map"][tag.toStdString()].get<std::string>()));
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