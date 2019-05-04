#include "GidiProcessor.h"
#include "GamepadMap.h"

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

int GidiProcessor::parseNote(String note) 
{

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

GidiProcessor::GidiProcessor() : Thread("ControllerProcessing") 
{
}

GidiProcessor::GidiProcessor(int controllerIndex, int mapIndex, MidiOutput* midi) 
                            : Thread("ControllerProcessing") 
{

    activeControllerIndex = controllerIndex;
    activeMapIndex = mapIndex;

    midiOut = std::unique_ptr<MidiOutput>(midi);
    midiState = new MidiKeyboardState();

    prevCompState.insert_or_assign(ComponentType::A, false);
    prevCompState.insert_or_assign(ComponentType::B, false);
    prevCompState.insert_or_assign(ComponentType::X, false);
    prevCompState.insert_or_assign(ComponentType::Y, false);
    prevCompState.insert_or_assign(ComponentType::DpadUp, false);
    prevCompState.insert_or_assign(ComponentType::DpadDown, false);
    prevCompState.insert_or_assign(ComponentType::DpadLeft, false);
    prevCompState.insert_or_assign(ComponentType::DpadRight, false);
    prevCompState.insert_or_assign(ComponentType::LBmpr, false);
    prevCompState.insert_or_assign(ComponentType::RBmpr, false);
    prevCompState.insert_or_assign(ComponentType::LStick, false);
    prevCompState.insert_or_assign(ComponentType::RStick, false);
    prevCompState.insert_or_assign(ComponentType::Start, false);
    prevCompState.insert_or_assign(ComponentType::Back, false);
    prevCompState.insert_or_assign(ComponentType::Guide, false);
    prevCompState.insert_or_assign(ComponentType::LTrigger, 0);
    prevCompState.insert_or_assign(ComponentType::RTrigger, 0);
    prevCompState.insert_or_assign(ComponentType::LStickX, 0);
    prevCompState.insert_or_assign(ComponentType::LStickY, 0);
    prevCompState.insert_or_assign(ComponentType::RStickX, 0);
    prevCompState.insert_or_assign(ComponentType::RStickY, 0);
}

GidiProcessor::~GidiProcessor() {
    stopThread(2000);
    
    if (componentMap != nullptr) {
        delete componentMap;
    }
    componentMap = nullptr; 
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
    if (controllerHandles[activeControllerIndex] != 0 && SDL_GameControllerGetAttached(controllerHandles[activeControllerIndex])) {
        recordControllerState();
        // Remember HERE to call pressure sensitive changes before button changes !!! (so velocity controls will take prio)
        //handleAxisMessages();
        // handleButtonChanges();
        handleComponentChanges();
        MidiBuffer buffer; // buffer to add our message queue to
        for (auto msg : msgQueue) {
            if (midiState != nullptr) {
                midiState->processNextMidiEvent(msg);
            }
            buffer.addEvent(msg, 1);
        }
        midiOut->startBackgroundThread();
        midiOut->sendBlockOfMessagesNow(buffer); // send buffered messages to MIDI out
        midiOut->stopBackgroundThread();
        msgQueue.clear();
    } 
    else {
        // game controller is not plugged in
        stopThread(250);
    } 
}


void GidiProcessor::recordControllerState() 
{
    SDL_GameController* controller = controllerHandles[activeControllerIndex];

    currCompState.insert_or_assign(ComponentType::A, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A));
    currCompState.insert_or_assign(ComponentType::B, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B));
    currCompState.insert_or_assign(ComponentType::X, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X));
    currCompState.insert_or_assign(ComponentType::Y, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y));
    currCompState.insert_or_assign(ComponentType::DpadUp, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP));
    currCompState.insert_or_assign(ComponentType::DpadDown, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN));
    currCompState.insert_or_assign(ComponentType::DpadLeft, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT));
    currCompState.insert_or_assign(ComponentType::DpadRight, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
    currCompState.insert_or_assign(ComponentType::LBmpr, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
    currCompState.insert_or_assign(ComponentType::RBmpr, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
    currCompState.insert_or_assign(ComponentType::LStick, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK));
    currCompState.insert_or_assign(ComponentType::RStick, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK));
    currCompState.insert_or_assign(ComponentType::Start, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START));
    currCompState.insert_or_assign(ComponentType::Back, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK));
    currCompState.insert_or_assign(ComponentType::Guide, (bool) SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_GUIDE));
    currCompState.insert_or_assign(ComponentType::LTrigger, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    currCompState.insert_or_assign(ComponentType::RTrigger, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
    currCompState.insert_or_assign(ComponentType::LStickX, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX));
    currCompState.insert_or_assign(ComponentType::LStickY, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY));
    currCompState.insert_or_assign(ComponentType::RStickX, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX));
    currCompState.insert_or_assign(ComponentType::RStickY, SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY));
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
                                msgQueue.add(MidiMessage::noteOn(1, note,(uint8)defaultVelocity));
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
                                msgQueue.add(MidiMessage::noteOff(1, note));
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
                        msgQueue.add(MidiMessage::pitchWheel(1, position));
                        break;
                    case ComponentSpecialFunctions::Velocity:
                        // DO Velocity stuff here
                        break;
                }
            }
        }
    }
}

void GidiProcessor::handleComponentChanges() {
    for (const auto& i : currCompState) {
        // if (compMap.count(i.first) == 0) { // if our component map contains the current key CHANGE ME AHH
            if (i.second.index() == 0) {  // if the variant is a bool, we are looking at a On/Off button state
                if (i.second != prevCompState[i.first]) { // button state has changed from prev call!
                    if (std::get<bool>(i.second)) { // Button got pressed on!
                    } 
                    else { // Button got turned off!
                    }
                }
            }        
            else { // we must be looking at a Pressure sensitive int

            }
        // }
        prevCompState[i.first] = i.second; // set the prev state to our current state
    }
}