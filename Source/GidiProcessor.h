/* 
    Copyright 2017-2020 Mason Dructor
*/
/*
    This file is part of Gidi.

    Gidi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gidi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gidi.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiMap.h"
#include "GamepadMap.h"
#include "ComponentType.h"
#include "GamepadComponent.h"
#include "NoteParser.h"
#include "ComponentSpecialFunction.h"
#include "AppSettings.h"
#include <memory>
#include <variant>

#define SDL_MAIN_HANDLED
#include <SDL.h>
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

        int midiChannel = 1;

        GidiMap activeMap;

        GamepadMap<std::variant<bool, int>> prevCompState;
        GamepadMap<std::variant<bool, int>> currCompState;
        GamepadMap<Array<int>> compMap;

        Array<int> notesOn;

        bool sustainOn = false;
        Array<int> notesSustained;

        Array<MidiMessage> msgQueue;

        void recordControllerState();
        void handleComponentChanges();

        int defaultVelocity = 100;
        int currentVelocity = defaultVelocity;
        int octaveChange = 0;
        int pitchChange = 0;

        std::unique_ptr<MidiOutput> midiOut;

        std::shared_ptr<MidiKeyboardState> midiState;

        std::shared_ptr<GamepadComponent> gamepadComponent;


    public:

        static Array<String> getCtrlrNames();
        static void updateCtrlrHandles();

        GidiProcessor();
        GidiProcessor(int controllerIndex, const GidiMap& map, std::unique_ptr<MidiOutput> midi);
        ~GidiProcessor();

        int getCurrentVelocity() { return currentVelocity; }
        void setDefaultVelocity(int velocity) { defaultVelocity = velocity; }

        int getOctaveChange() {return octaveChange;}
        void setOctaveChange(int val) {octaveChange = val;
                                       msgQueue.add(MidiMessage::allNotesOff(1));}
        int getPitchChange() {return pitchChange;}
        void setPitchChange(int val) {pitchChange = val;msgQueue.add(MidiMessage::allNotesOff(1));}

        std::shared_ptr<MidiKeyboardState> getBoardState() { return std::shared_ptr<MidiKeyboardState>(midiState); }
        void setBoardState( std::shared_ptr<MidiKeyboardState> state) { midiState = state;}

        GamepadMap<Array<int>> getComponentMap() { return compMap;}

        void setGamepadComponent(std::shared_ptr<GamepadComponent> gp) { gamepadComponent = gp; }

        void pulse();
        virtual void run() override;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GidiProcessor)
};

