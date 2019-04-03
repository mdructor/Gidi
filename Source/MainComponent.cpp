/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{    
    activeView->setContentOwned(new ActiveView(), true);

    btnRefresh.setButtonText("Refresh");
    btnRefresh.setImages(false, true, false, refreshImage, 1, Colours::blue, refreshImage, .5, Colours::blueviolet, refreshImage, .25, Colours::burlywood);
    btnRefresh.onClick = [this] { refresh(); };

    btnToggle.setButtonText("Start");
    btnToggle.setImages(false, true, false, playIcon, 1, Colours::grey, playIcon, .5, Colours::lightgrey, playIcon, .25, Colours::whitesmoke);
    btnToggle.onClick = [this] { toggle(); };

    btnSettings.setImages(false, true, false, settingsIcon, 1, Colours::grey, settingsIcon, .5, Colours::lightgrey, settingsIcon, .25, Colours::whitesmoke);

    cbMidiPorts.onChange = [this] { midiChanged(); };

    lblController.setText("Controller:", NotificationType::dontSendNotification);
    lblMapping.setText("Mapping:", NotificationType::dontSendNotification);
    lblMidiPort.setText("Midi Port:", NotificationType::dontSendNotification);

    addAndMakeVisible(btnRefresh);
    addAndMakeVisible(btnSettings);
    addAndMakeVisible(btnToggle);
    addAndMakeVisible(cbControllers);
    addAndMakeVisible(cbMappings);
    addAndMakeVisible(lblController);
    addAndMakeVisible(lblMapping);
    addAndMakeVisible(lblMidiPort);
    addAndMakeVisible(cbMidiPorts);

    refresh();

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 200);

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
    refresh();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    if (processor != nullptr) {
        delete processor;
        processor = nullptr;
    }
    if (midiOut != nullptr) {
        delete midiOut;
        midiOut = nullptr;
    }

    delete activeView;
    activeView = nullptr;

    shutdownAudio();
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
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    if (processor != nullptr) {
        processor->pulse();
        MidiBuffer buffer; // buffer to add our message queue to
        ActiveView* comp = (ActiveView*) activeView->getContentComponent();
        for (auto msg : *(processor->getMessageQueue())) {
            comp->sendVisualKeyboardMessage(msg);
            buffer.addEvent(msg, 1);
        }
        midiOut->startBackgroundThread();
        midiOut->sendBlockOfMessagesNow(buffer); // send buffered messages to MIDI out
        midiOut->stopBackgroundThread();
        processor->getMessageQueue()->clear();
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

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    lblMidiPort.setBounds(5, 25, 100, 25);
    cbMidiPorts.setBounds(100, 25, 250, 25);
    lblController.setBounds(5, 75, 100, 25);
    cbControllers.setBounds(100, 75, 250, 25);
    lblMapping.setBounds(5, 125, 100, 25);
    cbMappings.setBounds(100, 125, 250, 25);
    btnRefresh.setBounds(360, 80, 30, 30);
    btnSettings.setBounds(360, 20, 30, 30);
    btnToggle.setBounds(360, 140, 30, 30);
}

void MainComponent::refresh() {
    GidiProcessor::updateCtrlrHandles();
    StringArray names = GidiProcessor::ctrlrNames();
    StringArray cbNames;
    for (int i = 0; i < cbControllers.getNumItems(); ++i) {
        cbNames.add(cbControllers.getItemText(i));
    }

    bool diff = false;

    if (names.size() != cbNames.size()) {
        diff = true;
    }
    else {
        for (int i = 0; i < names.size(); ++i) {
            if (names[i] != cbNames[i]) {
                diff = true;
            }
        }
    }

    if (diff) {
        cbControllers.clear();
        int i = 1;
        for (auto name : names) {
            cbControllers.addItem(name, i);
            ++i;
        }
    }

    
    cbMappings.clear();
    int i = 1;
    for (auto name : mapReader.getLoadedMapNames()) {
        cbMappings.addItem(name, i);
        ++i;
    } 

    cbMidiPorts.clear();
    i = 1;
    for (auto name : MidiOutput::getDevices()) {
        cbMidiPorts.addItem(name, i);
        ++i;
    }
    // If LINUX OR MAC:: TODO::
    cbMidiPorts.addItem("Create new virtual port...", i);
}

void MainComponent::toggle() {
    if (cbControllers.getSelectedId() == 0 || cbMappings.getSelectedId() == 0 || cbMidiPorts.getSelectedId() == 0) {
        return;
    }

    if (processor == nullptr) { // must be off
        btnToggle.setButtonText("Stop");
        btnToggle.setImages(false, true, false, pauseIcon, 1, Colours::grey, pauseIcon, .5, Colours::lightgrey, pauseIcon, .25, Colours::whitesmoke);
        btnRefresh.setEnabled(false);
        cbMappings.setEnabled(false);
        cbControllers.setEnabled(false);
        cbMidiPorts.setEnabled(false);
        processor = new GidiProcessor(cbControllers.getSelectedId() - 1, mapReader.getComponentMap(cbMappings.getSelectedId() - 1));
        midiOut = MidiOutput::openDevice(cbMidiPorts.getSelectedId() - 1);
        if (midiOut == nullptr) {
            printf("Couldn't open midi device...\n");
        }
        activeView->setVisible(true);
        ActiveView* comp = (ActiveView*) activeView->getContentComponent();
        comp->setActiveProcessor(processor);
        comp->setMapInfo(mapReader.getMapInfo(cbMappings.getSelectedId() - 1));
    }
    else {
        ActiveView* comp = (ActiveView*) activeView->getContentComponent();
        comp->removeActiveProcessor();

        btnToggle.setButtonText("Start");
        btnToggle.setImages(false, true, false, playIcon, 1, Colours::grey, playIcon, .5, Colours::lightgrey, playIcon, .25, Colours::whitesmoke);
        btnRefresh.setEnabled(true);
        cbMappings.setEnabled(true);
        cbControllers.setEnabled(true);
        cbMidiPorts.setEnabled(true);

        delete processor;
        processor = nullptr;

        activeView->setVisible(false);
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
        refresh();
    }
}
