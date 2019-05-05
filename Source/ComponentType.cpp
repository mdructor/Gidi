#include "ComponentType.h"

String ComponentTypeToString (ComponentType ct) {
    switch (ct) {
        case ComponentType::A: return "A";
        case ComponentType::B: return "B";
        case ComponentType::X: return "X";
        case ComponentType::Y: return "Y";
        case ComponentType::DpadUp: return "D-pad Up";
        case ComponentType::DpadDown: return "D-pad Down";
        case ComponentType::DpadLeft: return "D-pad Left";
        case ComponentType::DpadRight: return "D-pad Right";
        case ComponentType::LBmpr: return "Left Bumper";
        case ComponentType::RBmpr: return "Right Bumper";
        case ComponentType::LStick: return "Left Stick";
        case ComponentType::RStick: return "Right Stick";
        case ComponentType::Start: return "Start";
        case ComponentType::Back: return "Back";
        case ComponentType::Guide: return "Guide";
        case ComponentType::LTrigger: return "Left Trigger";
        case ComponentType::RTrigger: return "Right Trigger";
        case ComponentType::LStickX: return "Left Stick X";
        case ComponentType::LStickY: return "Left Stick Y";
        case ComponentType::RStickX: return "Right Stick X";
        case ComponentType::RStickY: return "Right Stick Y";
        default:
            return "";
    }
}