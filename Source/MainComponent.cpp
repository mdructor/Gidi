#include "MainComponent.h"

MainComponent::MainComponent()
{   
    // Button setup
    btnRefresh.setImages(false, true, false, refreshImage, 1, Colours::white, refreshImage, .5, Colours::lightgrey, refreshImage, .25, Colours::whitesmoke);
    btnRefresh.onClick = [this] { refreshComboBoxes(); };
    btnToggle.setImages(false, true, false, playIcon, 1, Colours::white, playIcon, .5, Colours::lightgrey, playIcon, .25, Colours::whitesmoke);
    btnToggle.onClick = [this] { toggle(); };
    btnSettings.setImages(false, true, false, settingsIcon, 1, Colours::white, settingsIcon, .5, Colours::lightgrey, settingsIcon, .25, Colours::whitesmoke);
    btnSettings.onClick = [this] { openOptionsDialog(); };

    // Combo box listeners
    cbMidiPorts.onChange = [this] { midiChanged(); };

    // Label init
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
    for (auto btn : *gamepadComponent->ctrlrBtns) {
        btn->onStateChange = [this,btn] {onGamepadButtonStateChange(btn);};
    }

    // adding in sections, left column down, then right column down, then footer
    addAndMakeVisible(btnRefresh);
    addAndMakeVisible(btnSettings);
    addAndMakeVisible(btnToggle);
    addAndMakeVisible(cbControllers);
    addAndMakeVisible(cbMappings);
    addAndMakeVisible(lblController);
    addAndMakeVisible(lblMapping);
    addAndMakeVisible(lblMidiPort);
    addAndMakeVisible(cbMidiPorts);

    addAndMakeVisible(txtMapInfo);

    addAndMakeVisible(gamepadComponent);

    addAndMakeVisible(lblVelocity);
    addAndMakeVisible(lblOctave);
    addAndMakeVisible(lblPitch);
    addAndMakeVisible(sldrVelocity);
    addAndMakeVisible(sldrOctave);
    addAndMakeVisible(sldrPitch);

    addAndMakeVisible (midiVisual);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (585, 415);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    refreshComboBoxes();
}

MainComponent::~MainComponent()
{
    if (gamepadComponent != nullptr) {
        delete gamepadComponent;
    }
    // This shuts down the audio device and clears the audio source.
    if (processor != nullptr) {
        delete processor;
        processor = nullptr;
    }
    if (midiOut != nullptr) {
        delete midiOut;
        midiOut = nullptr;
    }

    delete midiVisual;

    shutdownAudio();
}

void MainComponent::resized()
{
    // Top Left Quadrant of Combo Boxes/labels
    lblMidiPort.setBounds(5, 15, 100, 25);
    cbMidiPorts.setBounds(80, 15, 200, 25);
    lblController.setBounds(5, 50, 100, 25);
    cbControllers.setBounds(80, 50, 200, 25);
    lblMapping.setBounds(5, 85, 100, 25);
    cbMappings.setBounds(80, 85, 200, 25);

    // Buttons underneath combo box section
    btnRefresh.setBounds(30, 130, 30, 30);
    btnSettings.setBounds(130, 130, 30, 30);
    btnToggle.setBounds(230, 130, 30, 30);

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
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    if (isProcessing) {
        processor->pulse();
        MidiBuffer buffer; // buffer to add our message queue to
        for (auto msg : *(processor->getMessageQueue())) {
            keyboardState.processNextMidiEvent(msg);
            buffer.addEvent(msg, 1);
        }
        midiOut->startBackgroundThread();
        midiOut->sendBlockOfMessagesNow(buffer); // send buffered messages to MIDI out
        midiOut->stopBackgroundThread();
        processor->getMessageQueue()->clear();
    } else {
        if (processor != nullptr) {
            delete processor;
            processor = nullptr;
        }
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    // For more details, see the help for AudioProcessor::releaseResources()
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

    names.clear();
    cbNames.clear();
    for (auto name : mapReader.getLoadedMapNames()) {
        names.add(name);
    }
    for (int i = 0; i < cbMappings.getNumItems(); ++i) {
        cbNames.add(cbMappings.getItemText(i));
    }
    if (names != cbNames) { // if they were different
        cbMappings.clear();
        int i = 1;
        for (auto name : names) {
            cbMappings.addItem(name, i);
            ++i;
        }
    }

    names.clear();
    cbNames.clear();
    for (auto name : MidiOutput::getDevices()) {
        names.add(name);
    }
    for (int i = 0; i < cbMidiPorts.getNumItems(); ++i) {
        cbNames.add(cbMidiPorts.getItemText(i));
    }
    cbNames.removeString("Create new virtual port...");
    int i = 1;
    if (names != cbNames) { // if they were different
        cbMidiPorts.clear();
        for (auto name : names) {
            cbMidiPorts.addItem(name, i);
            ++i;
        }
        cbMidiPorts.addItem("Create new virtual port...", i);
    }
}

void MainComponent::toggle() {
    if (cbControllers.getSelectedId() == 0 || cbMappings.getSelectedId() == 0 || cbMidiPorts.getSelectedId() == 0) {
        if (!isProcessing) {
            return;
        }
    }

    if (!isProcessing) { // must be off
        btnToggle.setImages(false, true, false, pauseIcon, 1, Colours::white, pauseIcon, .5, Colours::lightgrey, pauseIcon, .25, Colours::whitesmoke);
        btnRefresh.setEnabled(false);
        cbMappings.setEnabled(false);
        cbControllers.setEnabled(false);
        cbMidiPorts.setEnabled(false);
        sldrOctave.setEnabled(true);
        sldrPitch.setEnabled(true);
        sldrVelocity.setEnabled(true);
        gamepadComponent->setEnabled(true);
        GidiProcessor::updateCtrlrHandles();
        processor = new GidiProcessor(cbControllers.getSelectedId() - 1, mapReader.getComponentMap(cbMappings.getSelectedId() - 1));
        midiOut = MidiOutput::openDevice(cbMidiPorts.getSelectedId() - 1);
        if (midiOut == nullptr) {
            printf("Couldn't open midi device...\n");
        }
        MapReader::MapInfo mapInfo = mapReader.getMapInfo(cbMappings.getSelectedId() - 1);
        String info = "Map: " + mapInfo.name + "\n" + "Author: " + mapInfo.author + "\n";
        txtMapInfo.setText(info);

        sldrOctave.setValue(processor->getOctaveChange());
        sldrPitch.setValue(processor->getPitchChange());
        sldrVelocity.setValue(processor->getCurrentVelocity());

        processor->addChangeListener(this);

        isProcessing = true;
    }
    else {
        btnToggle.setButtonText("Start");
        btnToggle.setImages(false, true, false, playIcon, 1, Colours::white, playIcon, .5, Colours::lightgrey, playIcon, .25, Colours::whitesmoke);
        btnRefresh.setEnabled(true);
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
    }
}

void MainComponent::midiChanged() {

    if (cbMidiPorts.getSelectedId() == cbMidiPorts.getNumItems()) { // if they selected create new midi portA

        DialogWindow::LaunchOptions dialogOptions;
        dialogOptions.dialogTitle = "New virtual MIDI port";
        String* name = new String();
        dialogOptions.content.set(new NewMidiDialog(name), true);
        dialogOptions.launchAsync();
        if (name->isNotEmpty()) {
            virtualOuts.add(MidiOutput::createNewDevice(*name));
        }
        refreshComboBoxes();
    }
}

void MainComponent::openOptionsDialog() {
    DialogWindow::LaunchOptions dialogOptions;
    dialogOptions.dialogTitle = "Advanced Settings";
    dialogOptions.content.set(new OptionsComponent(), true);
    dialogOptions.launchAsync();
    mapReader.refresh();
    refreshComboBoxes();
}

void MainComponent::onSldrVelocityChange() {
    if (processor != nullptr) {
        processor->setDefaultVelocity(sldrVelocity.getValue());
    }
}

void MainComponent::onSldrOctaveChange() {
    if (processor != nullptr) {
        processor->setOctaveChange(sldrOctave.getValue());
    }
}

void MainComponent::onSldrPitchChange() {
    if (processor != nullptr) {
        processor->setPitchChange(sldrPitch.getValue());
    }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source) {
    sldrOctave.setValue(processor->getOctaveChange());
    sldrPitch.setValue(processor->getPitchChange());
    sldrVelocity.setValue(processor->getCurrentVelocity());
}

void MainComponent::onGamepadButtonStateChange(ControllerButton* source) {
    const StringArray searchTags = StringArray("A", "B", "X", "Y", "DpadUp", "DpadDown", "DpadLeft", "DpadRight", "LStick", "RStick", "RBmpr", "LBmpr", "Start", "Back", "Guide", "LTrigger", "RTrigger"); 
    if (source->isMouseOver()) {
        if (gamepadComponent->ctrlrBtns->containsValue(source)) {
            for (auto tag : searchTags) {
                if (gamepadComponent->ctrlrBtns->contains(tag)) {
                    if (gamepadComponent->ctrlrBtns->operator[](tag) == source) {
                        int compVal = processor->getcomponentMap()->operator[](tag);
                        String builder = tag + " Button:\n";
                        switch (compVal) {
                            case GidiProcessor::ComponentSpecialFunctions::OctaveDown:
                                builder += "Octave Down\n";
                                break;
                            case GidiProcessor::ComponentSpecialFunctions::OctaveUp:
                                builder += "Octave Up\n";
                                break;
                            case GidiProcessor::ComponentSpecialFunctions::PitchDown:
                                builder += "Pitch Down\n";
                                break;
                            case GidiProcessor::ComponentSpecialFunctions::PitchUp:
                                builder += "Pitch Up\n";
                                break;
                            case GidiProcessor::ComponentSpecialFunctions::Velocity:
                                builder += "Velocity\n";
                                break;
                            case GidiProcessor::ComponentSpecialFunctions::PitchBend:
                                builder += "Pitch Bend\n";
                                break;
                            default:
                                builder += "Note On: " + String(compVal) + "\n";
                        }
                        txtMapInfo.setText(builder);
                    }
                }
            }
        }
    }
    else {
        MapReader::MapInfo mapInfo = mapReader.getMapInfo(cbMappings.getSelectedId() - 1);
        String info = "Map: " + mapInfo.name + "\n" + "Author: " + mapInfo.author + "\n";
        txtMapInfo.setText(info);;
    }
}