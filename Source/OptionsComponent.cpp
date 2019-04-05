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

void OptionsComponent::onButtonCancel() {
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
        dw->exitModalState(-1);
    }
}

void OptionsComponent::onButtonSave() {
    AppSettings::setMapDirectory(txtMapDirectory.getText());
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