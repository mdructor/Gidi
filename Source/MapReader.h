#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiProcessor.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class MapReader {

    private:
        const String PATH = "/dev/gidi/Resources/mappings";
        const String PATH2 = "/home/mason/dev/gidi/Resources/mappings";
        Array<json> loadedMaps;
    

    public:
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

            if (rawMap["map"]["A"].is_string()) {
                componentMap->set("A", GidiProcessor::parseNote(rawMap["map"]["B"].get<std::string>()));
            }

            if (rawMap["map"]["B"].is_string()) {
                componentMap->set("B", GidiProcessor::parseNote(rawMap["map"]["B"].get<std::string>()));
            }


            return componentMap;
        }

};