#include "ActiveView.h"


void ActiveView::resized() 
{
    txtMapInfo.setBounds(325, 10, 195, 90);
    txtLog.setBounds(325, 110, 195, 180);

    sldrVelocity.setBounds(5, 190, 100, 100);
    sldrOctave.setBounds(115, 190, 100, 100);
    sldrPitch.setBounds(225, 190, 100, 100);

    lblVelocity.setBounds(5, 175, 100, 15);
    lblOctave.setBounds(115, 175, 100, 15);
    lblPitch.setBounds(225, 175, 100, 15);

    ctrlrBtns[0]->setBounds(235, 65, 30, 30); // right four
    ctrlrBtns[1]->setBounds(210, 90, 30, 30); // right four
    ctrlrBtns[2]->setBounds(260, 90, 30, 30); // right four
    ctrlrBtns[3]->setBounds(235, 115, 30, 30); // right four
    ctrlrBtns[4]->setBounds(130, 70, 25, 25); // guide button
    ctrlrBtns[5]->setBounds(85, 125, 45, 45); // left stick
    ctrlrBtns[6]->setBounds(165, 125, 45, 45); // right stick
    ctrlrBtns[7]->setBounds(55, 65, 20, 35); // up dpad
    ctrlrBtns[8]->setBounds(55, 110, 20, 35); // down dpad
    ctrlrBtns[9]->setBounds(25, 95, 35, 20); //left dpad
    ctrlrBtns[10]->setBounds(70, 95, 35, 20); // right dpad
    ctrlrBtns[11]->setBounds(60, 35, 50, 20); //leftbumper 
    ctrlrBtns[12]->setBounds(210, 35, 50, 20); // rightbumper 
    ctrlrBtns[13]->setBounds(60, 0, 50, 40); //left trigger 
    ctrlrBtns[14]->setBounds(210, 0, 50, 40); // right trigger 
}
        
void ActiveView::compBtnStateChanged(ControllerButton* source) {
    if (source->isMouseOver()) {
        if (source == ctrlrBtns[0]) {
            int compVal = activeProcessor->getButtonMap()->operator[]("Y");
            String builder = "Y Button:\n";
            switch (compVal) {
                case GidiProcessor::ButtonSpecialFunctions::OctaveDown:
                    builder += "Octave Down\n";
                    break;
                case GidiProcessor::ButtonSpecialFunctions::OctaveUp:
                    builder += "Octave Up\n";
                    break;
                case GidiProcessor::ButtonSpecialFunctions::PitchDown:
                    builder += "Pitch Down\n";
                    break;
                case GidiProcessor::ButtonSpecialFunctions::PitchUp:
                    builder += "Pitch Up\n";
                    break;
                default:
                    builder += "Note On: " + String(compVal) + "\n";
            }
            txtMapInfo.setText(builder);
        }
        else {
            txtMapInfo.setText("COMPONENT INFO");
        }
    }
    else {
        drawMapInfo();
    }
}

ActiveView::ActiveView() {
    sldrVelocity.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrVelocity.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrVelocity.setRange(0, 127, 1);
    sldrVelocity.onValueChange = [this] { onSldrVelocityChange(); };

    sldrOctave.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrOctave.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrOctave.setRange(-5, 5, 1);
    sldrOctave.onValueChange = [this] { onSldrOctaveChange(); };
    

    sldrPitch.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrPitch.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrPitch.setRange(-11, 11, 1);
    sldrPitch.onValueChange = [this] { onSldrPitchChange(); };

    txtMapInfo.setMultiLine(true);
    txtLog.setMultiLine(true);
    txtMapInfo.setReadOnly(true);
    txtLog.setReadOnly(true);

    lblVelocity.setText("Velocity", NotificationType::dontSendNotification);
    lblVelocity.setJustificationType(Justification::centred);
    lblOctave.setText("Octave", NotificationType::dontSendNotification);
    lblOctave.setJustificationType(Justification::centred);
    lblPitch.setText("Pitch", NotificationType::dontSendNotification);
    lblPitch.setJustificationType(Justification::centred);

    for (int i = 0; i < 7; ++i) {
        ctrlrBtns.add(new ControllerButton());
    }
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::DpadVert));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::DpadVert));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::DpadHori));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::DpadHori));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::Bumper));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::Bumper));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::Trigger));
    ctrlrBtns.add(new ControllerButton(ControllerButton::ButtonType::Trigger));

    for (ControllerButton* btn : ctrlrBtns) {
        btn->onStateChange = [this, btn] {compBtnStateChanged(btn);};
        addAndMakeVisible(btn);
    }

    addAndMakeVisible(txtMapInfo);
    addAndMakeVisible(txtLog);
    addAndMakeVisible(sldrVelocity);
    addAndMakeVisible(sldrOctave);
    addAndMakeVisible(sldrPitch);
    addAndMakeVisible(lblVelocity);
    addAndMakeVisible(lblOctave);
    addAndMakeVisible(lblPitch);

    GidiLogger::addListener(this);

    setSize(525, 300);
}

ActiveView::~ActiveView() 
{
    for (ControllerButton* btn: ctrlrBtns) {
        delete btn;
        btn = nullptr;
    }
    GidiLogger::removeListener(this);
}

void ActiveView::changeListenerCallback(ChangeBroadcaster* source) 
{
    for (auto msg : GidiLogger::logQueue) {
        txtLog.setText(txtLog.getText() + "\n" + msg);
    }
    GidiLogger::logQueue.clear();
    if (activeProcessor != nullptr) {
        sldrVelocity.setValue(activeProcessor->getCurrentVelocity());
        sldrOctave.setValue(activeProcessor->getOctaveChange());
        sldrPitch.setValue(activeProcessor->getPitchChange());
    }
    txtLog.moveCaretToEnd();
}

void ActiveView::setActiveProcessor(GidiProcessor* processor) {
    activeProcessor = processor;
}

void ActiveView::removeActiveProcessor() {
    activeProcessor = nullptr;
}

void ActiveView::onSldrVelocityChange() {
    if (activeProcessor != nullptr) {
        activeProcessor->setDefaultVelocity(sldrVelocity.getValue());
    }
}

void ActiveView::onSldrOctaveChange() {
    if (activeProcessor != nullptr) {
        activeProcessor->setOctaveChange(sldrOctave.getValue());
    }
}

void ActiveView::onSldrPitchChange() {
    if (activeProcessor != nullptr) {
        activeProcessor->setPitchChange(sldrPitch.getValue());
    }
}

void ActiveView::setMapInfo(MapReader::MapInfo mapInfo) {
    String info = "Map: " + mapInfo.name + "\n" + "Author: " + mapInfo.author + "\n";
    strMapInfo = info;
    drawMapInfo();
}

void ActiveView::drawMapInfo() {
    txtMapInfo.setText(strMapInfo);
}