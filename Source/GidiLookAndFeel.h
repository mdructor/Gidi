#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GidiLookAndFeel : public LookAndFeel_V4
{
    public:

        const Colour primary = Colour::fromRGB(45, 45, 45);
        const Colour secondary = Colour::fromRGB(80, 80, 80);
        const Colour accent = Colour::fromRGB(25, 25, 25);

        GidiLookAndFeel() {

            setColour(ResizableWindow::backgroundColourId, primary);

            setColour(ComboBox::backgroundColourId, secondary);
            
            setColour(PopupMenu::backgroundColourId, secondary);

            setColour(TextEditor::backgroundColourId, secondary);

            setColour(Slider::ColourIds::thumbColourId, accent);

            setColour(Slider::rotarySliderFillColourId, secondary);
            setColour(Slider::ColourIds::rotarySliderOutlineColourId, secondary);

        }

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiLookAndFeel)
};