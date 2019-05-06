#include "AppSettings.h"

String AppSettings::mapDirectory;
String AppSettings::iconsDirectory;
int AppSettings::midiChannel;

bool AppSettings::loadAppSettings() {
    File settingsFile = File::getCurrentWorkingDirectory().getChildFile("settings.json");
    if (!settingsFile.existsAsFile()) {
        printf("Couldn't find app settings file...\n");
        return false;
    }
    json parsedSettings = json::parse(settingsFile.loadFileAsString().toStdString());
    if (parsedSettings["map-directory"].is_string()) {
        mapDirectory = parsedSettings["map-directory"].get<std::string>();
    }
    else {
        mapDirectory = "";
    }

    if (parsedSettings["icons-directory"].is_string()) {
        iconsDirectory = parsedSettings["icons-directory"].get<std::string>();
    }
    else {
        iconsDirectory = "";
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