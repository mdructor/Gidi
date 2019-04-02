#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiLogger.h"
#include "GidiProcessor.h"
#include "ControllerButton.h"
#include "MapReader.h"

// https://wireframe.cc/7IqUMF

class ActiveView : public Component, public ChangeListener {

    private:

        TextEditor txtMapInfo;
        TextEditor txtLog;
        Slider     sldrVelocity;
        Slider     sldrOctave;
        Slider     sldrPitch;
        Label      lblVelocity;
        Label      lblOctave;
        Label      lblPitch;
        Array<ControllerButton*> ctrlrBtns;

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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveView)
};