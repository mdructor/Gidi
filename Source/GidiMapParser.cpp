#include "GidiMapParser.h"

// Note the following tags should correspond with ComponentType.h
const StringArray GidiMapParser::searchTags = 
            StringArray("A", 
                        "B", 
                        "X", 
                        "Y", 
                        "DpadUp", 
                        "DpadDown", 
                        "DpadLeft", 
                        "DpadRight", 
                        "LBmpr", 
                        "RBmpr", 
                        "LStick", 
                        "RStick", 
                        "Start", 
                        "Back",
                        "Guide", 
                        "LTrigger",
                        "RTrigger",
                        "LStickX",
                        "LStickY",
                        "RStickX",
                        "RStickY" 
                       );

GidiMap GidiMapParser::loadGidiMap(File mapFile) 
{
    GidiMap map;

    try {
        auto rawJson = json::parse(mapFile.loadFileAsString().toStdString());

        // Load Map Information
        if (!rawJson.contains("info") || !rawJson["info"].is_object()) {
            throw GidiMapParsingException("Map didn't contain info section.\n");
        }
        auto infoJson = rawJson["info"];

        if (infoJson.contains("name") && infoJson["name"].is_string()) {
            map.mapInfo.name = infoJson["name"].get<std::string>();
        }

        if (infoJson.contains("author") && infoJson["author"].is_string()) {
            map.mapInfo.name = infoJson["author"].get<std::string>();
        }

        if (infoJson.contains("default-velocity") && infoJson["default-velocity"].is_number_integer()) {
            map.mapInfo.defaultVelocity = infoJson["default-velocity"].get<int>();
        }

        // Load Component Map
        if (!rawJson.contains("map") || !rawJson["map"].is_object()) {
            throw GidiMapParsingException("Map didn't contain map section.\n");
        }
        auto mapJson = rawJson["map"];

        for (auto tag : searchTags) {
            Array<int> mappedFunctions;
            if (mapJson.contains(tag.toStdString())) {
                if (mapJson[tag.toStdString()].is_string()) { // component only holds 1 function
                    mappedFunctions.add(parseNote(mapJson[tag.toStdString()].get<std::string>()));
                }
                else if (mapJson[tag.toStdString()].is_array()) {
                    for (auto item : mapJson[tag.toStdString()].get<std::vector<std::string>>()) {
                        mappedFunctions.add(parseNote(item));
                    }
                }
            }
            if (mappedFunctions.contains(-1)) {
                throw GidiMapParsingException("Invalid note in map.\n");
            }
            else {
                if (mappedFunctions.size() > 0) {
                    ComponentType ct = (ComponentType) searchTags.indexOf(tag);
                    map.componentMap.insert_or_assign(ct, mappedFunctions);
                }
            }
        }

        return map;
    }
    catch (const json::parse_error& e) {
        throw GidiMapParsingException("Failed to parse json.\n");
    }

    return map;
}