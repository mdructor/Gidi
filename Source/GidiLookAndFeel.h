#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GidiLookAndFeel : public LookAndFeel_V4
{
    public:
        /* decent
        const Colour primary = Colour::fromRGB(80, 81, 96);
        const Colour secondary = Colour::fromRGB(104, 130, 158);
        const Colour accent = Colour::fromRGB(174, 189, 56);
        */

       /* decent
        const Colour primary = Colour::fromRGB(55, 94, 151);
        const Colour secondary = Colour::fromRGB(63, 104, 28);
        const Colour accent = Colour::fromRGB(255, 187, 0);
        */

        const Colour primary = Colour::fromRGB(76, 100, 245);
        const Colour secondary = Colour::fromRGB(52, 55, 124);
        const Colour accent = Colour::fromRGB(32, 148, 139);

        GidiLookAndFeel() {

            setColour(ResizableWindow::backgroundColourId, primary);

            setColour(ComboBox::backgroundColourId, secondary);
            
            setColour(PopupMenu::backgroundColourId, secondary);

            setColour(TextEditor::backgroundColourId, secondary);

            setColour(TextButton::ColourIds::buttonColourId, secondary);
            setColour(TextButton::ColourIds::buttonOnColourId, accent);

            setColour(Slider::ColourIds::thumbColourId, accent);

            setColour(Slider::rotarySliderFillColourId, secondary);
            setColour(Slider::ColourIds::rotarySliderOutlineColourId, secondary);

        }

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiLookAndFeel)
};