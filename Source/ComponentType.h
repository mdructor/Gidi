#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum class ComponentType {
    A, 
    B, 
    X, 
    Y, 
    DpadUp, 
    DpadDown, 
    DpadLeft, 
    DpadRight, 
    LBmpr, 
    RBmpr,
    LStick,
    RStick,
    Start,
    Back,
    Guide,
    LTrigger,
    RTrigger,
    LStickX,
    LStickY,
    RStickX,
    RStickY
};

String ComponentTypeToString(ComponentType ct);
