/* 
    Copyright 2017-2020 Mason Dructor
*/
/*
    This file is part of Gidi.

    Gidi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gidi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gidi.  If not, see <https://www.gnu.org/licenses/>.
*/

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
        static int midiChannel;

    public:
        static bool loadAppSettings();
        static bool saveAppSettings();

        static void setMapDirectory(String path);
        static String getMapDirectory();
        static String getIconsDirectory();

        static int getMidiChannel();
        static void setMidiChannel(int midiChannel);
};



