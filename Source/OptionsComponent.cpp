#include "OptionsComponent.h"

OptionsComponent::OptionsComponent() {

    lblMapDirectory.setText("Mappings Directory: ", NotificationType::dontSendNotification);

    txtMapDirectory.setReadOnly(true);

    btnCancel.setButtonText("Cancel");
    btnSave.setButtonText("Save");

    addAndMakeVisible(lblMapDirectory);
    addAndMakeVisible(txtMapDirectory);
    addAndMakeVisible(btnMapDirectory);
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

    btnCancel.setBounds(250, 160, 60, 30);
    btnSave.setBounds(330, 160, 60, 30);
}

void OptionsComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}