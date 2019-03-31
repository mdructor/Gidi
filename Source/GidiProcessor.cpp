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

Array<String> GidiProcessor::ctrlrNames() 
{
    Array<String> names;
    for (int i = 0; i < availableControllers; ++i) {
        names.add(SDL_GameControllerName(controllerHandles[i]));
    }
    return names;
}    

int GidiProcessor::parseNote(String note) {
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

GidiProcessor::GidiProcessor(int controllerIndex, HashMap<String, int>* componentMap) {
    initialise();

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
    if (buttonMap != nullptr) {
        delete buttonMap;
    }
    if (msgQueue!= nullptr) {
        delete msgQueue;
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
                            int note = buttonMap->operator[](i.getKey());
                            GidiLogger::logMsg("Sending note on message to " + String(note) + " at velocity 100");
                            msgQueue->add(MidiMessage::noteOn(1, note,(uint8)100));
                        }
                    }
                    else {
                        if (buttonMap->contains(i.getKey())) {
                            int note = buttonMap->operator[](i.getKey());
                            GidiLogger::logMsg("Sending note off message to " + String(note));
                            msgQueue->add(MidiMessage::noteOff(1, note));
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