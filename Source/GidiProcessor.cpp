#include "GidiProcessor.h"

SDL_GameController* GidiProcessor::controllerHandles[MAX_CONTROLLERS];
int GidiProcessor::availableControllers;


void GidiProcessor::updateCtrlrHandles() {
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
        return ComponentSpecialFunctions::OctaveUp; 
    }
    else if(note == "OctaveDown") {
        return ComponentSpecialFunctions::OctaveDown;
    }
    else if (note == "PitchUp") {
        return ComponentSpecialFunctions::PitchUp;
    }
    else if (note == "PitchDown") {
        return ComponentSpecialFunctions::PitchDown;
    }
    else if (note == "Velocity") {
        return ComponentSpecialFunctions::Velocity;
    }
    else if (note == "PitchBend") {
        return ComponentSpecialFunctions::PitchBend;
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

GidiProcessor::GidiProcessor() : Thread("ControllerProcessing") {
}

GidiProcessor::GidiProcessor(int controllerIndex, HashMap<String, Array<int>>* compMap, MidiOutput* midi) : Thread("ControllerProcessing") {

    activeControllerIndex = controllerIndex;
    componentMap = compMap;
    msgQueue = new Array<MidiMessage>();
    midiOut = midi;
    midiState = new MidiKeyboardState();
    
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

    prevAxisState.set("LTrigger", 0);
    prevAxisState.set("RTrigger", 0);
    prevAxisState.set("LStickX", 0);
    prevAxisState.set("LStickY", 0);
    prevAxisState.set("RStickY", 0);
    prevAxisState.set("RStickX", 0);
}

GidiProcessor::~GidiProcessor() {
    stopThread(2000);
    
    if (msgQueue != nullptr) {
        delete msgQueue;
    }
    if (componentMap != nullptr) {
        delete componentMap;
    }
    componentMap = nullptr; 
    msgQueue = nullptr;
    midiState = nullptr;
}

void GidiProcessor::run() {

    while (!threadShouldExit()) {
        wait(6);
        const MessageManagerLock mm1 (Thread::getCurrentThread());

        if (!mm1.lockWasGained()) {
            return ;
        }

        pulse();
    }

}

void GidiProcessor::pulse() {
    SDL_GameControllerUpdate();
    SDL_GameController* controller = controllerHandles[activeControllerIndex];
    if (controller != 0 && SDL_GameControllerGetAttached(controller)) {
        recordControllerState();
        // Remember HERE to call pressure sensitive changes before button changes !!! (so velocity controls will take prio)
        handleAxisMessages();
        handleButtonChanges();
        MidiBuffer buffer; // buffer to add our message queue to
        for (auto msg : *(getMessageQueue())) {
            if (midiState != nullptr) {
                midiState->processNextMidiEvent(msg);
            }
            buffer.addEvent(msg, 1);
        }
        midiOut->startBackgroundThread();
        midiOut->sendBlockOfMessagesNow(buffer); // send buffered messages to MIDI out
        midiOut->stopBackgroundThread();
        getMessageQueue()->clear();
    } 
    else {
        // game controller is not plugged in
        stopThread(250);
    } 
}

Array<MidiMessage>* GidiProcessor::getMessageQueue() {
    return msgQueue;
} 

void GidiProcessor::recordControllerState() {
    SDL_GameController* controller = controllerHandles[activeControllerIndex];
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

    currAxisState.set("LTrigger", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    currAxisState.set("RTrigger", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
    currAxisState.set("LStickX", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX));
    currAxisState.set("RStickX", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX));
    currAxisState.set("LStickY", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY));
    currAxisState.set("RStickY", SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY));
}

void GidiProcessor::handleButtonChanges() { // this is where we send MIDI messages based on button changes
    for (HashMap<String, bool>::Iterator i (currButtonState); i.next();) { // Loop through all buttons and check if state has changed
        String key = i.getKey();
        if (i.getValue() != prevButtonState[key]) {
            if (i.getValue()) { // if button just got turned on
                if (componentMap->contains(key)) {
                    for (int note : componentMap->operator[](key)) {
                        switch(note) {
                            case ComponentSpecialFunctions::OctaveUp:
                                setOctaveChange(getOctaveChange() + 1);
                                sendChangeMessage();
                                break;
                            case ComponentSpecialFunctions::OctaveDown:
                                setOctaveChange(getOctaveChange() - 1);
                                sendChangeMessage();
                                break;
                            case ComponentSpecialFunctions::PitchDown:
                                setPitchChange(getPitchChange() - 1);
                                sendChangeMessage();
                                break;
                            case ComponentSpecialFunctions::PitchUp:
                                setPitchChange(getPitchChange() + 1);
                                sendChangeMessage();
                                break;
                            default:
                                note += octaveChange * 12;
                                note += pitchChange;
                                msgQueue->add(MidiMessage::noteOn(1, note,(uint8)defaultVelocity));
                                break;
                        }
                    }
                }
            }
            else { // button just got turned off
                if (componentMap->contains(key)) {
                    for (int note : componentMap->operator[](key)) {
                        switch(note) {
                            case ComponentSpecialFunctions::OctaveUp:
                            case ComponentSpecialFunctions::OctaveDown:
                            case ComponentSpecialFunctions::PitchDown:
                            case ComponentSpecialFunctions::PitchUp:
                                break;
                            default:
                                note += octaveChange * 12;
                                note += pitchChange;
                                msgQueue->add(MidiMessage::noteOff(1, note));
                                break;
                        }
                    }
                }
            }
        }
        prevButtonState.set(i.getKey(), i.getValue()); // SET PREVIOUS STATE AS THE CURRENT STATE FOR NEXT PULSE!
    }
}

void GidiProcessor::handleAxisMessages() {
    for (HashMap<String, int>::Iterator i (currAxisState); i.next();) { // Loop through all axes and send messages a ccordingly 
        String key = i.getKey();

        if (componentMap->contains(key)) { 
            for (auto func : componentMap->operator[](key)) {
                float pct_down = i.getValue() / 32767.0;
                if (pct_down < 0) {
                    pct_down *= -1;
                }
                int position = (16383 * pct_down);
                switch (func) {
                    case ComponentSpecialFunctions::PitchBend:
                        // DO PITCH BEND STUFF HERE
                        msgQueue->add(MidiMessage::pitchWheel(1, position));
                        break;
                    case ComponentSpecialFunctions::Velocity:
                        // DO Velocity stuff here
                        break;
                }
            }
        }
    }
}