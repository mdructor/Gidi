#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppSettings.h"

// Component to edit the global app settings
class OptionsComponent : public Component 
{
    private:
        Label      lblMapDirectory;
        TextEditor txtMapDirectory;
        TextButton btnMapDirectory;
        TextButton     btnCancel;
        TextButton     btnSave;

        Label lblMidiChannel;
        Slider sldrMidiChannel;
        

        void resized() override;
        void paint(Graphics& g) override;

        // Button action handlers
        void onButtonCancel();
        void onButtonSave();
        void onMapDirectory();


    public:
        OptionsComponent();
        ~OptionsComponent();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OptionsComponent)
};