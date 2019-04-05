#include "GamepadComponent.h"

GamepadComponent::GamepadComponent() {
    ctrlrBtns = new HashMap<String, ControllerButton*>();

    ctrlrBtns->set("A", new ControllerButton());
    ctrlrBtns->set("B", new ControllerButton());
    ctrlrBtns->set("X", new ControllerButton());
    ctrlrBtns->set("Y", new ControllerButton());
    ctrlrBtns->set("LStick", new ControllerButton());
    ctrlrBtns->set("RStick", new ControllerButton());
    ctrlrBtns->set("Guide", new ControllerButton());
    ctrlrBtns->set("DpadLeft", 
                  new ControllerButton(
                            ControllerButton::ButtonType::DpadHori));
    ctrlrBtns->set("DpadRight",
                  new ControllerButton(
                            ControllerButton::ButtonType::DpadHori));
    ctrlrBtns->set("DpadUp",
                    new ControllerButton(
                            ControllerButton::ButtonType::DpadVert));
    ctrlrBtns->set("DpadDown",
                    new ControllerButton(
                            ControllerButton::ButtonType::DpadVert));
    ctrlrBtns->set("LBmpr",
                    new ControllerButton(
                            ControllerButton::ButtonType::Bumper));
    ctrlrBtns->set("RBmpr",
                    new ControllerButton(
                            ControllerButton::ButtonType::Bumper));
    ctrlrBtns->set("LTrigger",
                    new ControllerButton(
                            ControllerButton::ButtonType::Trigger));
    ctrlrBtns->set("RTrigger",
                    new ControllerButton(
                            ControllerButton::ButtonType::Trigger));
    ctrlrBtns->set("Start", 
                    new ControllerButton(ControllerButton::ButtonType::Bumper));

    ctrlrBtns->set("Back", 
                    new ControllerButton(ControllerButton::ButtonType::Bumper));
    
    for (ControllerButton* btn : *ctrlrBtns) {
        addAndMakeVisible(btn);
    }

    setSize(300, 175);
}

GamepadComponent::~GamepadComponent() {
    for (auto btn: *ctrlrBtns) {
        delete btn;
        btn = nullptr;
    }
    delete ctrlrBtns;
}

void GamepadComponent::resized() {
    ctrlrBtns->operator[]("Y")->setBounds(235, 65, 30, 30); // right four
    ctrlrBtns->operator[]("X")->setBounds(210, 90, 30, 30); // right four
    ctrlrBtns->operator[]("B")->setBounds(260, 90, 30, 30); // right four
    ctrlrBtns->operator[]("A")->setBounds(235, 115, 30, 30); // right four
    ctrlrBtns->operator[]("Guide")->setBounds(145, 70, 25, 25); // guide button
    ctrlrBtns->operator[]("LStick")->setBounds(95, 125, 45, 45); // left stick
    ctrlrBtns->operator[]("RStick")->setBounds(175, 125, 45, 45); // right stick
    ctrlrBtns->operator[]("DpadUp")->setBounds(55, 65, 20, 35); // up dpad
    ctrlrBtns->operator[]("DpadDown")->setBounds(55, 110, 20, 35); // down dpad
    ctrlrBtns->operator[]("DpadLeft")->setBounds(25, 95, 35, 20); //left dpad
    ctrlrBtns->operator[]("DpadRight")->setBounds(70, 95, 35, 20); // right dpad
    ctrlrBtns->operator[]("LBmpr")->setBounds(60, 35, 50, 20); //leftbumper 
    ctrlrBtns->operator[]("RBmpr")->setBounds(210, 35, 50, 20); // rightbumper 
    ctrlrBtns->operator[]("LTrigger")->setBounds(60, 0, 50, 40); //left trigger 
    ctrlrBtns->operator[]("RTrigger")->setBounds(210, 0, 50, 40);
    ctrlrBtns->operator[]("Start")->setBounds(175, 50, 30, 15);
    ctrlrBtns->operator[]("Back")->setBounds(115, 50, 30, 15);
}