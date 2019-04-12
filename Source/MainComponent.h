#pragma once

#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"
#include "MapReader.h"
#include "GidiProcessor.h"
#include "GamepadComponent.h"
#include "NewMidiDialog.h"
#include "AppSettings.h"
#include "OptionsComponent.h"

// Content of the main window -- holds control over the GidiProcessor

class MainComponent   : public Component, public ChangeListener
{
    private:
        ComboBox    cbControllers; 
        ComboBox    cbMappings;
        ComboBox    cbMidiPorts;
        ImageButton btnRefresh;
        ImageButton btnSettings;
        ImageButton btnToggle;
        Image refreshImage = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("refresh-button.png"));
        Image settingsIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("settings.png"));
        Image playIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("play-button.png")); // TODO: MOVE THIS LOAD ELSEWHERE
        Image pauseIcon = PNGImageFormat::loadFrom(File(AppSettings::getIconsDirectory()).getChildFile("pause-button.png"));
        Label       lblController;
        Label       lblMapping;
        Label       lblMidiPort;
        TextEditor txtMapInfo;
        MidiKeyboardState      keyboardState;
        Label  lblVelocity;
        Label  lblOctave;
        Label  lblPitch;
        Slider sldrVelocity;
        Slider sldrOctave;
        Slider sldrPitch;

        std::unique_ptr<MidiKeyboardComponent> midiVisual;
        std::unique_ptr<GamepadComponent> gamepadComponent;

        GidiProcessor* processor = nullptr;
        MidiOutput* midiOut = nullptr;
        Array<MidiOutput*> virtualOuts; // Need to fix this sometime
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
    public:
        MainComponent();
        ~MainComponent();

        void paint (Graphics& g) override;
        void resized() override;
        virtual void changeListenerCallback(ChangeBroadcaster* source) override;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
