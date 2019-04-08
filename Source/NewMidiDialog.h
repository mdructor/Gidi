#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Quick dialog to create a new virtual Midi port - feature only available on Linux/Mac

class NewMidiDialog : public Component {

    private:
        Label lblName;
        TextEditor txtName;
        TextButton btnCancel;
        TextButton btnCreate;

        String* nameToBe;

        void resized() override {
            lblName.setBounds(5, 15, 50, 25);
            txtName.setBounds(55, 15, 190, 25);
            btnCancel.setBounds(50, 45, 75, 25);
            btnCreate.setBounds(125, 45, 75, 25);
        }

        void paint (Graphics& g) override
        {
            g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        }

        void cancel() {
            if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
                dw->exitModalState(-1);
            }
        }

        void create() {
            if (txtName.isEmpty()) {
                return;
            }
            else {
                *nameToBe = txtName.getText();
                if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
                    dw->exitModalState(1);
                }
            }
        }

    public:
        NewMidiDialog(String* name) {
            nameToBe = name;
            lblName.setText("Name:", NotificationType::dontSendNotification);

            btnCancel.setButtonText("Cancel");
            btnCreate.setButtonText("Create");

            addAndMakeVisible(lblName);
            addAndMakeVisible(txtName);
            addAndMakeVisible(btnCancel);
            addAndMakeVisible(btnCreate);

            btnCancel.onClick = [this] {cancel();};
            btnCreate.onClick = [this] {create();};

            setSize(250, 75);
        }
        ~NewMidiDialog() {}


   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewMidiDialog)
};
