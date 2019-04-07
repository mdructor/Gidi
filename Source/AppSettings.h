#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class AppSettings {

    private:
        static String mapDirectory;
        static String iconsDirectory;

    public:
        static bool loadAppSettings();
        static bool saveAppSettings();

        static void setMapDirectory(String path);
        static String getMapDirectory();
        static String getIconsDirectory();
};



