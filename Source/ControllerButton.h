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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>

/* 
    special gui button component that will take on the shape of a 
    specified gamepad button.
*/

class ControllerButton : public DrawableButton {

    public:
        enum ButtonType {Circle, DpadVert, DpadHori, Bumper, Trigger};

    private:
        const Colour COMPONENT_OFF = Colour::fromRGBA(0, 0, 0, 0);
        const Colour COMPONENT_ON = Colours::red;
        ButtonType btnType;
        std::unique_ptr<Path> shape;
        std::unique_ptr<DrawablePath> path;
        std::unique_ptr<DrawablePath> over;
        std::unique_ptr<DrawablePath> down;

    public:
        ControllerButton(ButtonType btnType = ButtonType::Circle) : DrawableButton("", DrawableButton::ButtonStyle::ImageFitted) {
            shape = std::unique_ptr<Path>(new Path());
            if (btnType == ButtonType::Circle) {
                shape->addEllipse(0, 0, 50, 50);
            }
            else if (btnType == ButtonType::DpadVert) {
                shape->addRectangle(0, 0, 20, 50);
            }
            else if (btnType == ButtonType::DpadHori) {
                shape->addRectangle(0, 0, 50, 20);
            }
            else if (btnType == ButtonType::Bumper) {
                shape->addRoundedRectangle(0, 0, 50, 20, 5);
            }
            else if (btnType == ButtonType::Trigger) {
                shape->addRoundedRectangle(0, 0, 30, 35, 5);
            }

            path = std::unique_ptr<DrawablePath>(new DrawablePath());
            path->setPath(*shape);
            path->setStrokeThickness(3);
            path->setFill(FillType(COMPONENT_OFF));

            over = std::unique_ptr<DrawablePath>(new DrawablePath(*path));
            over->setStrokeFill(FillType(Colours::white));

            down = std::unique_ptr<DrawablePath>(new DrawablePath(*path));
            down->setFill(FillType(Colours::red));

            setImages(path.get(), over.get(), down.get());
        }
        ~ControllerButton() {
        }

        void setComponentState(bool on) {
            Colour fill;
            if (on) {
                fill = COMPONENT_ON;
            }
            else {
                fill = COMPONENT_OFF;
            }
            path->setFill(FillType(fill));
            over->setFill(FillType(fill));
            down->setFill(FillType(fill));
            setImages(path.get(), over.get(), down.get());
        }

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControllerButton)
};