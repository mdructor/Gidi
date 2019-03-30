#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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

};