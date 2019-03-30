/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MapReader.h"
#include "GidiProcessor.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    ComboBox   cbControllers; 
    ComboBox   cbMappings;
    ComboBox   cbMidiPorts;
    TextButton btnRefresh;
    TextButton btnToggle;
    Label      lblController;
    Label      lblMapping;
    Label      lblMidiPort;

    GidiProcessor* processor = nullptr;
    MidiOutput* midiOut = nullptr;
    MapReader mapReader;

    void refresh();
    void toggle();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
