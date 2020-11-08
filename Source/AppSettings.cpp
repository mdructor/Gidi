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

#include "AppSettings.h"

String AppSettings::mapDirectory;
String AppSettings::iconsDirectory;
int AppSettings::midiChannel;

bool AppSettings::loadAppSettings() {

    File settingsFile = File::getCurrentWorkingDirectory().getChildFile("settings.json");

    if (!settingsFile.existsAsFile()) {
        printf("Couldn't find app settings file...\n");
        printf("Going with defaults settings.\n");
        mapDirectory = "mappings/";
        iconsDirectory = "icons/";
        midiChannel = 1;
        return false;
    }

    json parsedSettings;

    try {
        parsedSettings = json::parse(settingsFile.loadFileAsString().toStdString());
    }
    catch (const json::parse_error& e) {
        printf("Couldn't parse settings file.");
        printf("Going with defaults settings.\n");
        mapDirectory = "mappings/";
        iconsDirectory = "icons/";
        midiChannel = 1;
    }

    if (parsedSettings["map-directory"].is_string()) {
        mapDirectory = parsedSettings["map-directory"].get<std::string>();
    }
    else {
        mapDirectory = "./mappings/";
    }

    if (parsedSettings["icons-directory"].is_string()) {
        iconsDirectory = parsedSettings["icons-directory"].get<std::string>();
    }
    else {
        iconsDirectory = "./icons/";
    }

    midiChannel = 0;
    if (parsedSettings["midi-channel"].is_number_integer()) {
        midiChannel = parsedSettings["midi-channel"].get<int>();
    }
    if (midiChannel < 1 || midiChannel > 15) {
        midiChannel = 1;
    }

    return true;
}
 
bool AppSettings::saveAppSettings() {
    return false;
}

void AppSettings::setMapDirectory(String path) {
    mapDirectory = path;
}

String AppSettings::getMapDirectory() {
    return mapDirectory;
}

String AppSettings::getIconsDirectory() {
    return iconsDirectory;
}


int AppSettings::getMidiChannel() {
    return midiChannel;
}

void AppSettings::setMidiChannel(int c) {
    if (c < 1 || c > 15) {
        return;
    }
    midiChannel = c;
}