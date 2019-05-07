#include "OptionsComponent.h"

OptionsComponent::OptionsComponent() {

    lblMapDirectory.setText("Mappings Directory: ", NotificationType::dontSendNotification);

    txtMapDirectory.setReadOnly(true);
    txtMapDirectory.setText(AppSettings::getMapDirectory());

    btnMapDirectory.onClick = [this] {onMapDirectory();};

    btnCancel.setButtonText("Cancel");
    btnCancel.onClick = [this] { onButtonCancel(); };

    btnSave.setButtonText("Save");
    btnSave.onClick = [this] { onButtonSave(); };

    lblMidiChannel.setText("Midi Channel:", NotificationType::dontSendNotification);

    sldrMidiChannel.setSliderStyle(Slider::SliderStyle::LinearBar);
    sldrMidiChannel.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 30, 25);
    sldrMidiChannel.setRange(1, 15, 1);
    sldrMidiChannel.setValue(AppSettings::getMidiChannel());

    lblVirtualMidi.setText("Create Midi Out:", NotificationType::dontSendNotification);

    btnVirtualMidi.setButtonText("Create");
    btnVirtualMidi.onClick = [this] { onButtonMidiCreate(); };


    addAndMakeVisible(lblMapDirectory);
    addAndMakeVisible(txtMapDirectory);
    addAndMakeVisible(btnMapDirectory);

    addAndMakeVisible(lblMidiChannel);
    addAndMakeVisible(sldrMidiChannel);

    addAndMakeVisible(lblVirtualMidi);
    addAndMakeVisible(txtVirtualMidi);
    addAndMakeVisible(btnVirtualMidi);

    addAndMakeVisible(btnCancel);
    addAndMakeVisible(btnSave);

    setSize(400, 200);
}

OptionsComponent::~OptionsComponent() {

}

void OptionsComponent::resized() {
    lblMapDirectory.setBounds(5, 10, 100, 25);
    txtMapDirectory.setBounds(105, 10, 250, 25);
    btnMapDirectory.setBounds(365, 10, 30, 25);

    lblMidiChannel.setBounds(5, 50, 100, 25);
    sldrMidiChannel.setBounds(105, 50, 290, 25);

    lblVirtualMidi.setBounds(5, 90, 100, 25);
    txtVirtualMidi.setBounds(125, 90, 200, 25);
    btnVirtualMidi.setBounds(335, 90, 60, 25);

    btnCancel.setBounds(250, 160, 60, 30);
    btnSave.setBounds(330, 160, 60, 30);
}

void OptionsComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void OptionsComponent::onButtonCancel() {
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
        dw->exitModalState(-1);
    }
}

void OptionsComponent::onButtonSave() {
    AppSettings::setMapDirectory(txtMapDirectory.getText());
    AppSettings::setMidiChannel(sldrMidiChannel.getValue());
    AppSettings::saveAppSettings();
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
        dw->exitModalState(1);
    }
}

void OptionsComponent::onMapDirectory() {
    FileChooser fileChooser("Select mappings directory...", File::getSpecialLocation(File::userDocumentsDirectory), "*");

    if (fileChooser.browseForDirectory()) {
        txtMapDirectory.setText(fileChooser.getResult().getFullPathName());
    }
}

void OptionsComponent::onButtonMidiCreate() 
{
    if (txtVirtualMidi.getText() == "") {
        return;
    }
    else {
        String name = txtVirtualMidi.getText();
        #if JUCE_LINUX || JUCE_MAC
            auto midiout = MidiOutput::createNewDevice(name);
            if (midiout == nullptr) {
                printf("Failed to create virtual midi out.\n");
            }
            else {
                printf("Created virtual midi out!\n");
                txtVirtualMidi.clear();
            }
        #endif
    }
}