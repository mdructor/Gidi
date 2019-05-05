#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerButton.h"
#include "GamepadMap.h"
#include "ComponentType.h"

/* 
    UI component that holds a collection of buttons
    that look and relate to a generic gamepad
*/

class GamepadComponent : public Component
{
    private:
        void resized() override;

    public:
        GamepadComponent();
        ~GamepadComponent();

        GamepadMap<ControllerButton*>* ctrlrBtns;



JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GamepadComponent)
};