/* 
    Copyright 2017-2020 Mason Dructor
*/
/*
    This file is part of Gidi.

    Gidi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gidi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gidi.  If not, see <https://www.gnu.org/licenses/>.
*/

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