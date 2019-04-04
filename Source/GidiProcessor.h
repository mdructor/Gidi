#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiLogger.h"
#include <SDL2/SDL.h>

class GidiProcessor : public ChangeBroadcaster {

    private:
        const static int MAX_CONTROLLERS = 4;
        static SDL_GameController* controllerHandles[MAX_CONTROLLERS];
        static int availableControllers;

        int activeControllerIndex = -1;

        HashMap<String, bool> prevButtonState;
        HashMap<String, bool> currButtonState;
        HashMap<String, int>* buttonMap;

        Array<MidiMessage>* msgQueue;

        int defaultVelocity = 100;
        int octaveChange = 0;
        int pitchChange = 0;

    public:

        enum ButtonSpecialFunctions {PitchUp=1, PitchDown=2, OctaveUp=3, OctaveDown=4};

        static int parseNote(String note);
        static Array<String> getCtrlrNames();
        static void updateCtrlrHandles();

        GidiProcessor();
        GidiProcessor(int controllerIndex, HashMap<String, int>* mapping, int defaultVelocity = 100);
        ~GidiProcessor();

        int getCurrentVelocity() {return defaultVelocity;}
        void setDefaultVelocity(int velocity) {defaultVelocity = velocity;}

        int getOctaveChange() {return octaveChange;}
        void setOctaveChange(int val) {octaveChange = val;}
        int getPitchChange() {return pitchChange;}
        void setPitchChange(int val) {pitchChange = val;}

        HashMap<String, int>* getButtonMap() {return buttonMap;}

        void pulse();
        Array<MidiMessage>* getMessageQueue();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiProcessor)
};

