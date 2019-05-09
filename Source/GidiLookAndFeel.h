#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GidiLookAndFeel : public LookAndFeel_V4
{
    public:

        GidiLookAndFeel() {

            setColour(ResizableWindow::backgroundColourId, Colour::fromRGB(45, 45, 45));

        }

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiLookAndFeel)
};