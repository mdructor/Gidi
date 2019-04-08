#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// This class acts like a singleton - will hold our global application
// data. Should call loadAppSettings at application load. 
//
// NOTE: Currently looks for settings.json in the same directory
//       as the executable. Definitely something to look into in 
//       the future  TODO TODO TODO

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



