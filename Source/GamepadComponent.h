#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerButton.h"


class GamepadComponent : public Component
{
    private:
        void resized() override;

    public:
        GamepadComponent();
        ~GamepadComponent();

        HashMap<String, ControllerButton*>* ctrlrBtns;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GamepadComponent)
};