#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppSettings.h"

class OptionsComponent : public Component 
{
    private:
        Label      lblMapDirectory;
        TextEditor txtMapDirectory;
        TextButton btnMapDirectory;

        TextButton     btnCancel;
        TextButton     btnSave;

        void resized() override;
        void paint(Graphics& g) override;

        void onButtonCancel();
        void onButtonSave();
        void onMapDirectory();


    public:
        OptionsComponent();
        ~OptionsComponent();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OptionsComponent)
};