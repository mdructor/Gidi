#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GamepadMap.h"

class GidiMap {

    private:

    public:

        struct MapInfo {
            String name;
            String author;
            int    defaultVelocity;
        };

        MapInfo mapInfo;
        GamepadMap<Array<int>> componentMap;

};