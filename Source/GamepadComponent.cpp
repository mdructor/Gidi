#include "GamepadComponent.h"

GamepadComponent::GamepadComponent() {

    ctrlrBtns = std::unique_ptr<GamepadMap<std::unique_ptr<ControllerButton>>>(new GamepadMap<std::unique_ptr<ControllerButton>>());
    ctrlrBtns->insert_or_assign(ComponentType::A, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::B, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::X, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::Y, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::LStick, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::RStick, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::Guide, std::unique_ptr<ControllerButton>(new ControllerButton()));
    ctrlrBtns->insert_or_assign(ComponentType::DpadLeft, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::DpadHori)));
    ctrlrBtns->insert_or_assign(ComponentType::DpadRight, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::DpadHori)));
    ctrlrBtns->insert_or_assign(ComponentType::DpadDown, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::DpadVert)));
    ctrlrBtns->insert_or_assign(ComponentType::DpadUp, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::DpadVert)));
    ctrlrBtns->insert_or_assign(ComponentType::LBmpr, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Bumper)));
    ctrlrBtns->insert_or_assign(ComponentType::RBmpr, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Bumper)));
    ctrlrBtns->insert_or_assign(ComponentType::LTrigger, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Trigger)));
    ctrlrBtns->insert_or_assign(ComponentType::RTrigger, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Trigger)));
    ctrlrBtns->insert_or_assign(ComponentType::Start, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Bumper)));
    ctrlrBtns->insert_or_assign(ComponentType::Back, std::unique_ptr<ControllerButton>(new ControllerButton(ControllerButton::ButtonType::Bumper)));

    for (const auto& i : *ctrlrBtns) {
        addAndMakeVisible(i.second.get());
    }

    setSize(300, 175);
}

GamepadComponent::~GamepadComponent() {
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