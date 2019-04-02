#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiLogger.h"
#include "GidiProcessor.h"
#include "ControllerButton.h"
#include "MapReader.h"

// https://wireframe.cc/7IqUMF

class ActiveView : public Component, public ChangeListener {

    private:

        // Using this as a universal controller button ID
        const StringArray searchTags = StringArray("A", "B", "X", "Y", "DpadUp", "DpadDown", "DpadLeft", 
                                                    "DpadRight", "LStick", "RStick", "RBmpr", "LBmpr", "Start", "Back", "Guide"); 
        // GUI Stuff
        TextEditor txtMapInfo;
        TextEditor txtLog;
        Slider     sldrVelocity;
        Slider     sldrOctave;
        Slider     sldrPitch;
        Label      lblVelocity;
        Label      lblOctave;
        Label      lblPitch;
        HashMap<String, ControllerButton*>* ctrlrBtns;
        MidiKeyboardState keyboardState;
        MidiKeyboardComponent* midiVisual = new MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard);


        GidiProcessor* activeProcessor = nullptr;

        int activeMap = -1;
        String strMapInfo;

        void resized() override; 
        void compBtnStateChanged(ControllerButton* source);

    public:

        ActiveView();
        ~ActiveView();

        virtual void changeListenerCallback(ChangeBroadcaster* source) override;

        void setActiveProcessor(GidiProcessor* processor);
        void removeActiveProcessor();
        void onSldrVelocityChange();
        void onSldrOctaveChange();
        void onSldrPitchChange();
        void setMapInfo(MapReader::MapInfo mapInfo);
        void drawMapInfo();
        void sendVisualKeyboardMessage(const MidiMessage& message);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveView)
};