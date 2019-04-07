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
#include "GamepadComponent.h"
#include "NewMidiDialog.h"
#include "AppSettings.h"
#include "OptionsComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public ChangeListener
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
    virtual void changeListenerCallback(ChangeBroadcaster* source) override;

private:

    ComboBox    cbControllers; 
    ComboBox    cbMappings;
    ComboBox    cbMidiPorts;
    ImageButton btnRefresh;
    ImageButton btnSettings;
    ImageButton btnToggle;
    Label       lblController;
    Label       lblMapping;
    Label       lblMidiPort;
    TextEditor txtMapInfo;
    MidiKeyboardState      keyboardState;
    MidiKeyboardComponent* midiVisual = new MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard);
    GamepadComponent* gamepadComponent = new GamepadComponent();
    Label  lblVelocity;
    Label  lblOctave;
    Label  lblPitch;
    Slider sldrVelocity;
    Slider sldrOctave;
    Slider sldrPitch;

    Image refreshImage = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("refresh-button.png"));
    Image settingsIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("settings.png"));
    Image playIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("play-button.png"));
    Image pauseIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("pause-button.png"));

    GidiProcessor* processor = nullptr;
    MidiOutput* midiOut = nullptr;
    Array<MidiOutput*> virtualOuts;
    MapReader mapReader;        

    bool isProcessing = false;

    void refreshComboBoxes();
    void toggle();
    void midiChanged();

    void openOptionsDialog();

    void onSldrVelocityChange();
    void onSldrOctaveChange();
    void onSldrPitchChange();

    void onGamepadButtonStateChange(ControllerButton* src);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
