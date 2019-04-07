#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class GidiProcessor : public ChangeBroadcaster {

    private:
        const static int MAX_CONTROLLERS = 4;
        static SDL_GameController* controllerHandles[MAX_CONTROLLERS];
        static int availableControllers;

        int activeControllerIndex = -1;

        HashMap<String, bool> prevButtonState;
        HashMap<String, bool> currButtonState;
        HashMap<String, int> prevAxisState;
        HashMap<String, int> currAxisState;
        HashMap<String, Array<int>>* componentMap;

        Array<MidiMessage>* msgQueue;

        void handleButtonChanges();
        void recordControllerState();
        void handleAxisMessages();

        int defaultVelocity = 100;
        int octaveChange = 0;
        int pitchChange = 0;

    public:

        enum ComponentSpecialFunctions {PitchUp=1, PitchDown=2, OctaveUp=3, OctaveDown=4, Velocity=5, PitchBend=6};

        static int parseNote(String note);
        static Array<String> getCtrlrNames();
        static void updateCtrlrHandles();

        GidiProcessor();
        GidiProcessor(int controllerIndex, HashMap<String, Array<int>>* mapping, int defaultVelocity = 100);
        ~GidiProcessor();

        int getCurrentVelocity() {return defaultVelocity;}
        void setDefaultVelocity(int velocity) {defaultVelocity = velocity;}

        int getOctaveChange() {return octaveChange;}
        void setOctaveChange(int val) {octaveChange = val;
                                       msgQueue->add(MidiMessage::allNotesOff(1));}
        int getPitchChange() {return pitchChange;}
        void setPitchChange(int val) {pitchChange = val;msgQueue->add(MidiMessage::allNotesOff(1));}

        HashMap<String, Array<int>>* getcomponentMap() {return componentMap;}

        void pulse();
        Array<MidiMessage>* getMessageQueue();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiProcessor)
};

