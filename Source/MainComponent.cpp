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

#include "MainComponent.h"

MainComponent::MainComponent()
{   
    keyboardState = std::shared_ptr<MidiKeyboardState>(new MidiKeyboardState());
    midiVisual = std::unique_ptr<MidiKeyboardComponent>(
        new MidiKeyboardComponent(*keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard));

    gamepadComponent = std::shared_ptr<GamepadComponent>(new GamepadComponent());

    // Button setup
    btnToggle.setImages(false, 
                        true, 
                        false, 
                        playIcon, 
                        1, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId), 
                        playIcon, 
                        .5, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId), 
                        playIcon, 
                        .25, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId));

    btnToggle.onClick = [this] { toggle(); };
    btnSettings.setImages(false, 
                        true, 
                        false, 
                        settingsIcon, 
                        1, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId), 
                        settingsIcon, 
                        .5, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId), 
                        settingsIcon, 
                        .25, 
                        getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId));
    btnSettings.onClick = [this] { openOptionsDialog(); };

    // Logo Setup
    imgLogo.setImage(logoIcon);



    // Label init
    fntTitle = Font("Consolas", 44, Font::plain);
    lblTitle.setText("Gidi", NotificationType::dontSendNotification);
    lblTitle.setFont(fntTitle);
    lblController.setText("Controller:", NotificationType::dontSendNotification);
    lblMapping.setText("Mapping:", NotificationType::dontSendNotification);
    lblMidiPort.setText("Midi Port:", NotificationType::dontSendNotification);
    lblOctave.setText("Octave", NotificationType::dontSendNotification);
    lblVelocity.setText("Velocity", NotificationType::dontSendNotification);
    lblPitch.setText("Pitch", NotificationType::dontSendNotification);
    lblOctave.setJustificationType(Justification::centred);
    lblVelocity.setJustificationType(Justification::centred);
    lblPitch.setJustificationType(Justification::centred);

    // Setup sliders
    sldrVelocity.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrVelocity.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrVelocity.setRange(0, 127, 1);
    sldrVelocity.setEnabled(false);
    sldrVelocity.onValueChange = [this] { onSldrVelocityChange(); };

    sldrOctave.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrOctave.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrOctave.setRange(-5, 5, 1);
    sldrOctave.setEnabled(false);
    sldrOctave.onValueChange = [this] { onSldrOctaveChange(); };
    

    sldrPitch.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sldrPitch.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    sldrPitch.setRange(-11, 11, 1);
    sldrPitch.setEnabled(false);
    sldrPitch.onValueChange = [this] { onSldrPitchChange(); };

    txtMapInfo.setMultiLine(true);
    txtMapInfo.setReadOnly(true);
    txtMapInfo.setText("Welcome to Gidi!\nSelect a midi port, controller, and mapping then press the play button to begin!\n");

    gamepadComponent->setEnabled(false);
    for (const auto& i : *(gamepadComponent->ctrlrBtns)) {
        ControllerButton* btn = i.second.get();
        btn->onStateChange = [this, btn] {onGamepadButtonStateChange(btn);};
    }

    // adding in sections, left column down, then right column down, then footer
    addAndMakeVisible(lblTitle);
    addAndMakeVisible(imgLogo);
    addAndMakeVisible(btnSettings);
    addAndMakeVisible(btnToggle);
    addAndMakeVisible(cbControllers);
    addAndMakeVisible(cbMappings);
    addAndMakeVisible(lblController);
    addAndMakeVisible(lblMapping);
    addAndMakeVisible(lblMidiPort);
    addAndMakeVisible(cbMidiPorts);

    addAndMakeVisible(txtMapInfo);

    addAndMakeVisible(*gamepadComponent);

    addAndMakeVisible(lblVelocity);
    addAndMakeVisible(lblOctave);
    addAndMakeVisible(lblPitch);
    addAndMakeVisible(sldrVelocity);
    addAndMakeVisible(sldrOctave);
    addAndMakeVisible(sldrPitch);

    addAndMakeVisible (*midiVisual);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (585, 415);

    refreshComboBoxes();
    startTimer(REFRESH_TIME);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
    // Buttons underneath combo box section
    lblTitle.setBounds(75, 5, 100, 50);
    imgLogo.setBounds(10, 0, 50, 50);
    btnSettings.setBounds(175, 10, 40, 40);
    btnToggle.setBounds(235, 10, 40, 40);

    // Top Left Quadrant of Combo Boxes/labels
    lblMidiPort.setBounds(5, 65, 100, 25);
    cbMidiPorts.setBounds(80, 65, 200, 25);
    lblController.setBounds(5, 100, 100, 25);
    cbControllers.setBounds(80, 100, 200, 25);
    lblMapping.setBounds(5, 135, 100, 25);
    cbMappings.setBounds(80, 135, 200, 25);

    // text boxes below buttons on left side
    txtMapInfo.setBounds(10, 175, 270, 128);

    // keyboard component at footer
    midiVisual->setBounds(10, 315, 565, 90);

    // gamepad component at top right
    gamepadComponent->setBounds(280, 15, 290, 175);

    // sliders on right
    lblVelocity.setBounds(290, 200, 80, 15);
    lblOctave.setBounds(390, 200, 80, 15);
    lblPitch.setBounds(490, 200, 80, 15);
    sldrVelocity.setBounds(290, 215, 80, 80);
    sldrOctave.setBounds(390, 215, 80, 80);
    sldrPitch.setBounds(490, 215, 80, 80);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}


void MainComponent::refreshComboBoxes() {

    StringArray names = GidiProcessor::getCtrlrNames(); // get list of current connected controllers
    StringArray cbNames; // populate list of controllers we have in available in the combo box
    for (int i = 0; i < cbControllers.getNumItems(); ++i) {
        cbNames.add(cbControllers.getItemText(i));
    }
    // ctrlrDiff the lists to see if they are different
    if (names != cbNames) { // if they were
        cbControllers.clear();
        int i = 1;
        for (auto name : names) {
            cbControllers.addItem(name, i);
            ++i;
        }
    }

   refreshMapComboBox();

    names.clear();
    cbNames.clear();
    for (auto device : MidiOutput::getAvailableDevices()) {
        names.add(device.name);
    }
    for (int i = 0; i < cbMidiPorts.getNumItems(); ++i) {
        cbNames.add(cbMidiPorts.getItemText(i));
    }
    int i = 1;
    if (names != cbNames) { // if they were different
        cbMidiPorts.clear();
        for (auto name : names) {
            cbMidiPorts.addItem(name, i);
            ++i;
        }
    }
}

void MainComponent::refreshMapComboBox() {
    StringArray cbNames;
    StringArray newNames;
    for (int i = 0; i < cbMappings.getNumItems(); ++i) {
        cbNames.add(cbMappings.getItemText(i));
    }

    File mapDir = File::getCurrentWorkingDirectory().getChildFile(AppSettings::getMapDirectory());
    if (mapDir.exists()) {
        auto childDirs = mapDir.findChildFiles(File::TypesOfFileToFind::findDirectories, false);
        Array<GidiMap> maps = GidiMapParser::loadMapsFromDir(mapDir);
        for (auto map : maps) {
            newNames.add(map.mapInfo.name);
        }
        for (auto dir : childDirs) {
            maps = GidiMapParser::loadMapsFromDir(dir);
            for (auto map : maps) {
                newNames.add(map.mapInfo.name);
            }
        }
    }
    if (newNames != cbNames) { // something changed, reload mapz!
        loadedMaps.clear();
        cbMappings.clear();
        if (mapDir.exists()) {
            auto childDirs = mapDir.findChildFiles(File::TypesOfFileToFind::findDirectories, false);
            Array<GidiMap> maps = GidiMapParser::loadMapsFromDir(mapDir);
            for (auto map : maps) {
                loadedMaps.add(map);
            }
            int id = 1;
            for (auto map : maps) {
                cbMappings.addItem(map.mapInfo.name, id);
                ++id;
            }
            for (auto dir : childDirs) {
                maps = GidiMapParser::loadMapsFromDir(dir);
                for (auto map : maps) {
                    loadedMaps.add(map);
                }
                cbMappings.addSeparator();
                cbMappings.addSectionHeading(dir.getFileName());
                cbMappings.addSeparator();
                for (auto map : maps) {
                    cbMappings.addItem(map.mapInfo.name, id);
                    ++id;
                }
            }
        }
    }
}

void MainComponent::toggle() {
    if (cbControllers.getSelectedId() == 0 || cbMappings.getSelectedId() == 0 || cbMidiPorts.getSelectedId() == 0) {
        if (!isProcessing) {
            return;
        }
    }

    if (!isProcessing) { // must be off
        stopTimer();
        btnToggle.setImages(false, 
                            true, 
                            false, 
                            pauseIcon, 
                            1, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId), 
                            pauseIcon, 
                            .5, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId), 
                            pauseIcon, 
                            .25, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId));

        cbMappings.setEnabled(false);
        cbControllers.setEnabled(false);
        cbMidiPorts.setEnabled(false);
        sldrOctave.setEnabled(true);
        sldrPitch.setEnabled(true);
        sldrVelocity.setEnabled(true);
        gamepadComponent->setEnabled(true);

        GidiMap activeMap = loadedMaps[cbMappings.getSelectedId() - 1];
        String info = "Map: " + activeMap.mapInfo.name + "\n" + "Author: " + activeMap.mapInfo.author + "\n";
        txtMapInfo.setText(info);

        GidiProcessor::updateCtrlrHandles();
        processor = std::unique_ptr<GidiProcessor>(new GidiProcessor(cbControllers.getSelectedId() - 1, loadedMaps[cbMappings.getSelectedId() - 1], MidiOutput::openDevice(MidiOutput::getAvailableDevices()[cbMidiPorts.getSelectedId() - 1].identifier)));

        sldrOctave.setValue(processor->getOctaveChange());
        sldrPitch.setValue(processor->getPitchChange());
        sldrVelocity.setValue(processor->getCurrentVelocity());

        isProcessing = true;
        processor->addChangeListener(this);
        processor->setBoardState(keyboardState);
        processor->setGamepadComponent(gamepadComponent);
        processor->startThread(10);
    }
    else {
        startTimer(REFRESH_TIME);
        btnToggle.setImages(false, 
                            true, 
                            false, 
                            playIcon, 
                            1, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonColourId), 
                            playIcon, 
                            .5, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId), 
                            playIcon, 
                            .25, 
                            getLookAndFeel().findColour(TextButton::ColourIds::buttonOnColourId));

        cbMappings.setEnabled(true);
        cbControllers.setEnabled(true);
        cbMidiPorts.setEnabled(true);

        sldrOctave.setEnabled(false);
        sldrPitch.setEnabled(false);
        sldrVelocity.setEnabled(false);
        gamepadComponent->setEnabled(false);

        txtMapInfo.setText("Welcome to Gidi!\nSelect a midi port, controller, and mapping then press the play button to begin!\n");

        processor->removeChangeListener(this);
        isProcessing = false;
        if (processor) {
            processor.reset();
        }
    }
}

void MainComponent::openOptionsDialog() {
    DialogWindow::LaunchOptions dialogOptions;
    dialogOptions.dialogTitle = "Advanced Settings";
    dialogOptions.content.set(new OptionsComponent(), true);
    dialogOptions.content->setLookAndFeel(&getLookAndFeel());
    dialogOptions.launchAsync();
    refreshComboBoxes();
}

void MainComponent::onSldrVelocityChange() {
    if (processor) {
        processor->setDefaultVelocity(sldrVelocity.getValue());
    }
}

void MainComponent::onSldrOctaveChange() {
    if (processor) {
        processor->setOctaveChange(sldrOctave.getValue());
    }
}

void MainComponent::onSldrPitchChange() {
    if (processor) {
        processor->setPitchChange(sldrPitch.getValue());
    }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source) {
    sldrOctave.setValue(processor->getOctaveChange());
    sldrPitch.setValue(processor->getPitchChange());
    sldrVelocity.setValue(processor->getCurrentVelocity());
}

void MainComponent::onGamepadButtonStateChange(ControllerButton* source) {
    if (source->isMouseOver()) {
        for (const auto& i : *(gamepadComponent->ctrlrBtns)) {
            if (i.second.get() == source) {
                String builder = ComponentTypeToString(i.first) + ":\n";
                GamepadMap<Array<int>> gpm = processor->getComponentMap();
                if (gpm.count(i.first) != 0) {
                    for (const auto& n : gpm.operator[](i.first)) {
                        if (n < 20) { // TODO : May need to change this for max functions DEFINITELY SOMETIME
                            builder += ComponentSpecialFunctionToString((ComponentSpecialFunction) n) + "\n";
                        }
                        else {
                            builder += "Note On: " + MidiMessage::getMidiNoteName(n, true, true, 4) + "\n";
                        }
                    }

                }
                txtMapInfo.setText(builder);
                break;
            }
        }
    }
    else {
        String info = "Map: " + loadedMaps[cbMappings.getSelectedId() - 1].mapInfo.name + "\n" + "Author: " + loadedMaps[cbMappings.getSelectedId() - 1].mapInfo.author + "\n";
        txtMapInfo.setText(info);;
    }
}

void MainComponent::timerCallback() 
{
    refreshComboBoxes();
}