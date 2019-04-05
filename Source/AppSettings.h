#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class AppSettings {

    private:
        static String mapDirectory;

    public:
        static bool loadAppSettings();
        static bool saveAppSettings();

        static void setMapDirectory(String path);
        static String getMapDirectory();
};



