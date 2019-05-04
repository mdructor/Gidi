#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GamepadMap.h"
#include "ComponentType.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <memory>
#include <variant>

/*
    Core of Gidi 
    This static part of this class holds a handle to all available gamepads connected 
    on a system. You can retrieve the list of names of these controllers statically.
    Once you have chosen the controller you want to connect to, a mapping, and a midi out
    you can create an instance of the processor.

    This is will instantiate everything you need to start processing by using
    startThread(int prio). Then gidi will run in a seperate thread, processing controller
    input and sending generated midi messages to the designated midi out. 
*/

class GidiProcessor : public Thread, public ChangeBroadcaster {

    private:
        const static int MAX_CONTROLLERS = 4;
        static SDL_GameController* controllerHandles[MAX_CONTROLLERS];
        static int availableControllers;

        int activeControllerIndex = -1;
        int activeMapIndex = -1;

        HashMap<String, bool> prevButtonState;
        HashMap<String, bool> currButtonState;
        HashMap<String, int> prevAxisState;
        HashMap<String, int> currAxisState;
        HashMap<String, Array<int>>* componentMap;

        GamepadMap<std::variant<bool, int>> prevCompState;
        GamepadMap<std::variant<bool, int>> currCompState;
        GamepadMap<Array<int>> compMap;

        Array<MidiMessage> msgQueue;

        void handleButtonChanges();
        void recordControllerState();
        void handleAxisMessages();
        void handleComponentChanges();

        int defaultVelocity = 100;
        int octaveChange = 0;
        int pitchChange = 0;

        std::unique_ptr<MidiOutput> midiOut;

        MidiKeyboardState* midiState = nullptr;


    public:

        enum ComponentSpecialFunctions {PitchUp=1, PitchDown=2, OctaveUp=3, OctaveDown=4, Velocity=5, PitchBend=6};

        static int parseNote(String note);
        static Array<String> getCtrlrNames();
        static void updateCtrlrHandles();

        GidiProcessor();
        GidiProcessor(int controllerIndex, int mapIndex, MidiOutput* midiOut);
        ~GidiProcessor();

        int getCurrentVelocity() {return defaultVelocity;}
        void setDefaultVelocity(int velocity) {defaultVelocity = velocity;}

        int getOctaveChange() {return octaveChange;}
        void setOctaveChange(int val) {octaveChange = val;
                                       msgQueue.add(MidiMessage::allNotesOff(1));}
        int getPitchChange() {return pitchChange;}
        void setPitchChange(int val) {pitchChange = val;msgQueue.add(MidiMessage::allNotesOff(1));}

        MidiKeyboardState* getBoardState() { return midiState; }
        void setBoardState( MidiKeyboardState* state) { midiState = state;}

        HashMap<String, Array<int>>* getcomponentMap() {return componentMap;}

        void pulse();
        virtual void run() override;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiProcessor)
};

