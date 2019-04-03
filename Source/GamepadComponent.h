#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerButton.h"


class GamepadComponent : public Component
{
    private:
        void resized() override;

        HashMap<String, ControllerButton*>* ctrlrBtns;
    public:
        GamepadComponent();
        ~GamepadComponent();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GamepadComponent)
};