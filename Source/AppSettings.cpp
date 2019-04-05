#include "AppSettings.h"

String AppSettings::mapDirectory;

bool AppSettings::loadAppSettings() {
    File settingsFile = File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory).getChildFile("Gidi/settings.json");
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