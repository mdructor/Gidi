#include "GidiProcessor.h"

SDL_GameController* GidiProcessor::controllerHandles[MAX_CONTROLLERS];
int GidiProcessor::availableControllers;
bool GidiProcessor::initialised;

void GidiProcessor::initialise() {
    if (!initialised) {
        SDL_Init(SDL_INIT_GAMECONTROLLER);
        initialised = true;
    }
}

void GidiProcessor::updateCtrlrHandles() {
    initialise();
    int cIndex = 0;
    for (int jIndex=0; jIndex < SDL_NumJoysticks(); ++jIndex) {
        if (!SDL_IsGameController(jIndex)) {
            continue;
        }
        if (cIndex >= MAX_CONTROLLERS) {
            break;
        }
        controllerHandles[cIndex] = SDL_GameControllerOpen(jIndex);
        ++cIndex;
    }
    availableControllers = cIndex;
}

Array<String> GidiProcessor::getCtrlrNames() 
{
    initialise();
    Array<String> names;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            names.add(SDL_GameControllerNameForIndex(i));
        }
    }
    return names;
}    

int GidiProcessor::parseNote(String note) {

    // Also handle parsing Special Button Functions here. probably a bad idea but can 
    // fix it later
    // TODO:: ME
    if (note == "OctaveUp") {
        return ButtonSpecialFunctions::OctaveUp; 
    }
    else if(note == "OctaveDown") {
        return ButtonSpecialFunctions::OctaveDown;
    }
    else if (note == "PitchUp") {
        return ButtonSpecialFunctions::PitchUp;
    }
    else if (note == "PitchDown") {
        return ButtonSpecialFunctions::PitchDown;
    }

    int result = 0; 
    note = note.trim();
    if (note.length() < 2 || note.length() > 3) { // invalid note string
        return -1;
    } 
    note = note.toLowerCase(); // can at least be NOT case-sensitive
    auto first = note[0];
    if (first < 'a' || first > 'g') { // first char must be a note between a - g
        return -1;
    }
    switch (first) {
        case 'a': result = 33; break;
        case 'b': result = 35; break;
        case 'c': result = 24; break;
        case 'd': result = 26; break;
        case 'e': result = 28; break;
        case 'f': result = 29; break;
        case 'g': result = 31; break;
    }
    auto second = note[1];
    if (note.length() == 2) {
        if (second < '1' || second > '8') {
            return -1;
        }
        else {
            result += (12 * (second - '1'));
        }
    }
    else {
        if (second == 'b') {
            --result;
        }
        else if (second == '#') {
            ++result;
        }
        else {
            return -1;
        }
        auto third = note[2];
        if (third < '1' || third > '8') {
            return -1;
        }
        else {
            result += (12 * (third - '1'));
        }
    }
    return result;
}

GidiProcessor::GidiProcessor() {
    initialise();
}

GidiProcessor::GidiProcessor(int controllerIndex, HashMap<String, int>* componentMap, int defVel) {
    initialise();

    defaultVelocity = defVel;

    activeControllerIndex = controllerIndex;
    buttonMap = componentMap;
    msgQueue = new Array<MidiMessage>();
    
    prevButtonState.set("A", false);
    prevButtonState.set("B", false);
    prevButtonState.set("X", false); 
    prevButtonState.set("Y", false); 
    prevButtonState.set("DpadUp", false); 
    prevButtonState.set("DpadDown", false);
    prevButtonState.set("DpadLeft", false);
    prevButtonState.set("DpadRight", false);
    prevButtonState.set("LBmpr", false);
    prevButtonState.set("RBmpr", false);
    prevButtonState.set("LStick", false); 
    prevButtonState.set("RStick", false);
    prevButtonState.set("Start", false);
    prevButtonState.set("Back", false);
    prevButtonState.set("Guide", false);
}

GidiProcessor::~GidiProcessor() {
    
    if (msgQueue != nullptr) {
        delete msgQueue;
    }
    if (buttonMap != nullptr) {
        delete buttonMap;
    }
    buttonMap = nullptr; 
    msgQueue = nullptr;
}

void GidiProcessor::pulse() {
    SDL_GameControllerUpdate();
    SDL_GameController* controller = controllerHandles[activeControllerIndex];
    if (controller != 0 && SDL_GameControllerGetAttached(controller)) {
        currButtonState.set("A", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A));
        currButtonState.set("B", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B));
        currButtonState.set("X", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X));
        currButtonState.set("Y", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y));
        currButtonState.set("DpadUp", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP));
        currButtonState.set("DpadDown", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN));
        currButtonState.set("DpadLeft", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
        currButtonState.set("DpadRight", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
        currButtonState.set("LBmpr", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
        currButtonState.set("RBmpr", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
        currButtonState.set("LStick", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK));
        currButtonState.set("RStick", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK));
        currButtonState.set("Start", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START));
        currButtonState.set("Back", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK));
        currButtonState.set("Guide", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_GUIDE));

        for (HashMap<String, bool>::Iterator i (currButtonState); i.next();) { // Loop through all buttons and check if state has changed
            String key = i.getKey();
            if (prevButtonState.contains(key)) {
                if (i.getValue() != prevButtonState[key]) {
                    if (i.getValue()) {
                        if (buttonMap->contains(key)) {
                            int note = buttonMap->operator[](key);
                            switch(note) {
                                case ButtonSpecialFunctions::OctaveUp:
                                    GidiLogger::logMsg("Octave up message sent");
                                    ++octaveChange;
                                    sendChangeMessage();
                                    break;
                                case ButtonSpecialFunctions::OctaveDown:
                                    GidiLogger::logMsg("Octave down message sent");
                                    --octaveChange;
                                    sendChangeMessage();
                                    break;
                                case ButtonSpecialFunctions::PitchDown:
                                    GidiLogger::logMsg("Pitch down message sent");
                                    --pitchChange;
                                    sendChangeMessage();
                                    break;
                                case ButtonSpecialFunctions::PitchUp:
                                    GidiLogger::logMsg("Pitch up message sent");
                                    ++pitchChange;
                                    sendChangeMessage();
                                    break;
                                default:
                                    note += octaveChange * 12;
                                    note += pitchChange;
                                    GidiLogger::logMsg("Note on: " + String(note) + " @ velocity: " + String(defaultVelocity));
                                    msgQueue->add(MidiMessage::noteOn(1, note,(uint8)defaultVelocity));
                                    break;
                            }
                        }
                    }
                    else {
                        if (buttonMap->contains(key)) {
                            int note = buttonMap->operator[](i.getKey());
                            switch(note) {
                                case ButtonSpecialFunctions::OctaveUp:
                                case ButtonSpecialFunctions::OctaveDown:
                                case ButtonSpecialFunctions::PitchDown:
                                case ButtonSpecialFunctions::PitchUp:
                                    break;
                                default:
                                    note += octaveChange * 12;
                                    note += pitchChange;
                                    GidiLogger::logMsg("Note off: " + String(note));
                                    msgQueue->add(MidiMessage::noteOff(1, note));
                                    break;
                            }
                        }
                    }
                }
            }
            prevButtonState.set(i.getKey(), i.getValue());
        }

    } 
    else {
        // game controller is not plugged in
    } 
}

Array<MidiMessage>* GidiProcessor::getMessageQueue() {
    return msgQueue;
} 