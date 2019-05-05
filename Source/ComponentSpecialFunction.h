#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum class ComponentSpecialFunction 
{
    PitchUp = 1, 
    PitchDown = 2, 
    OctaveUp = 3, 
    OctaveDown = 4, 
    Velocity = 5, 
    PitchBend = 6
};

String ComponentSpecialFunctionToString(ComponentSpecialFunction csf);