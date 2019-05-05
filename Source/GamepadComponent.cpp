#include "GamepadComponent.h"

GamepadComponent::GamepadComponent() {

    ctrlrBtns = new GamepadMap<ControllerButton*>();
    ctrlrBtns->insert_or_assign(ComponentType::A, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::B, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::X, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::Y, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::LStick, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::RStick, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::Guide, new ControllerButton());
    ctrlrBtns->insert_or_assign(ComponentType::DpadLeft, new ControllerButton(ControllerButton::ButtonType::DpadHori));
    ctrlrBtns->insert_or_assign(ComponentType::DpadRight, new ControllerButton(ControllerButton::ButtonType::DpadHori));
    ctrlrBtns->insert_or_assign(ComponentType::DpadDown, new ControllerButton(ControllerButton::ButtonType::DpadVert));
    ctrlrBtns->insert_or_assign(ComponentType::DpadUp, new ControllerButton(ControllerButton::ButtonType::DpadVert));
    ctrlrBtns->insert_or_assign(ComponentType::LBmpr, new ControllerButton(ControllerButton::ButtonType::Bumper));
    ctrlrBtns->insert_or_assign(ComponentType::RBmpr, new ControllerButton(ControllerButton::ButtonType::Bumper));
    ctrlrBtns->insert_or_assign(ComponentType::LTrigger, new ControllerButton(ControllerButton::ButtonType::Trigger));
    ctrlrBtns->insert_or_assign(ComponentType::RTrigger, new ControllerButton(ControllerButton::ButtonType::Trigger));
    ctrlrBtns->insert_or_assign(ComponentType::Start, new ControllerButton(ControllerButton::ButtonType::Bumper));
    ctrlrBtns->insert_or_assign(ComponentType::Back, new ControllerButton(ControllerButton::ButtonType::Bumper));

    for (auto i : *ctrlrBtns) {
        addAndMakeVisible(i.second);
    }

    setSize(300, 175);
}

GamepadComponent::~GamepadComponent() {
    for (auto i: *ctrlrBtns) {
        delete i.second;
        i.second = nullptr;
    }
    delete ctrlrBtns;
    ctrlrBtns = nullptr;
}

void GamepadComponent::resized() {
    ctrlrBtns->operator[](ComponentType::Y)->setBounds(235, 65, 30, 30); // right four
    ctrlrBtns->operator[](ComponentType::X)->setBounds(210, 90, 30, 30); // right four
    ctrlrBtns->operator[](ComponentType::B)->setBounds(260, 90, 30, 30); // right four
    ctrlrBtns->operator[](ComponentType::A)->setBounds(235, 115, 30, 30); // right four
    ctrlrBtns->operator[](ComponentType::Guide)->setBounds(145, 70, 25, 25); // guide button
    ctrlrBtns->operator[](ComponentType::LStick)->setBounds(95, 125, 45, 45); // left stick
    ctrlrBtns->operator[](ComponentType::RStick)->setBounds(175, 125, 45, 45); // right stick
    ctrlrBtns->operator[](ComponentType::DpadUp)->setBounds(55, 65, 20, 35); // up dpad
    ctrlrBtns->operator[](ComponentType::DpadDown)->setBounds(55, 110, 20, 35); // down dpad
    ctrlrBtns->operator[](ComponentType::DpadLeft)->setBounds(25, 95, 35, 20); //left dpad
    ctrlrBtns->operator[](ComponentType::DpadRight)->setBounds(70, 95, 35, 20); // right dpad
    ctrlrBtns->operator[](ComponentType::LBmpr)->setBounds(60, 35, 50, 20); //leftbumper 
    ctrlrBtns->operator[](ComponentType::RBmpr)->setBounds(210, 35, 50, 20); // rightbumper 
    ctrlrBtns->operator[](ComponentType::LTrigger)->setBounds(60, 0, 50, 40); //left trigger 
    ctrlrBtns->operator[](ComponentType::RTrigger)->setBounds(210, 0, 50, 40);
    ctrlrBtns->operator[](ComponentType::Start)->setBounds(175, 50, 30, 15);
    ctrlrBtns->operator[](ComponentType::Back)->setBounds(115, 50, 30, 15);
}