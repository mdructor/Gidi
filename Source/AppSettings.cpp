#include "AppSettings.h"

String AppSettings::mapDirectory;
String AppSettings::iconsDirectory;

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