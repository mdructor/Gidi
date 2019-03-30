#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <SDL2/SDL.h>

class GidiProcessor {

    private:
        const static int MAX_CONTROLLERS = 4;
        static SDL_GameController* controllerHandles[MAX_CONTROLLERS];
        static int availableControllers;
        static bool initialised;
        static void initialise();

        int activeControllerIndex = -1;
        int activeMappingIndex = -1;

        HashMap<String, bool> prevButtonState;
        HashMap<String, bool> currButtonState;
        HashMap<String, int> buttonMap;

        Array<MidiMessage> msgQueue;

    public:
        static int parseNote(String note);
        static Array<String> ctrlrNames();
        static void updateCtrlrHandles();

        GidiProcessor();
        GidiProcessor(int controllerIndex, int mappingIndex);
        ~GidiProcessor();

        void pulse();
        void getMessageQueue();
};

