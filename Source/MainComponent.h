/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

/* NEW UI REWRITE: https://wireframe.cc/Q4Cq0n */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MapReader.h"
#include "GidiProcessor.h"
#include "NewMidiDialog.h"
#include "ActiveView.h"

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
    ComboBox   cbControllers; 
    ComboBox   cbMappings;
    ComboBox   cbMidiPorts;
    ImageButton btnRefresh;
    ImageButton btnSettings;
    ImageButton btnToggle;
    Label      lblController;
    Label      lblMapping;
    Label      lblMidiPort;

    Image      refreshImage = PNGImageFormat::loadFrom(File("/home/mason/dev/mdGidi/Resources/icons/refresh-button.png"));
    Image      settingsIcon = PNGImageFormat::loadFrom(File("/home/mason/dev/mdGidi/Resources/icons/settings.png"));
    Image      playIcon = PNGImageFormat::loadFrom(File("/home/mason/dev/mdGidi/Resources/icons/play-button.png"));
    Image      pauseIcon = PNGImageFormat::loadFrom(File("/home/mason/dev/mdGidi/Resources/icons/pause-button.png"));

    GidiProcessor* processor = nullptr;
    MidiOutput* midiOut = nullptr;
    Array<MidiOutput*> virtualOuts;
    MapReader mapReader;        

    DocumentWindow* activeView = new DocumentWindow("mdGidi - Controller View", getLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                                         DocumentWindow::TitleBarButtons::minimiseButton, true);

    void refresh();
    void toggle();
    void midiChanged();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
