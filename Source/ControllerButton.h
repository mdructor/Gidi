#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class ControllerButton : public DrawableButton {

    public:
        enum ButtonType {Circle, DpadVert, DpadHori, Bumper, Trigger};

    private:
        ButtonType btnType;
        Path* shape;
        DrawablePath* path;
        DrawablePath* over;
        DrawablePath* down;

    public:
        ControllerButton(ButtonType btnType = ButtonType::Circle) : DrawableButton("", DrawableButton::ButtonStyle::ImageFitted) {
            shape = new Path();
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

            path = new DrawablePath();
            path->setPath(*shape);
            path->setStrokeThickness(3);
            path->setFill(FillType(Colour::fromRGBA(0, 0, 0, 0)));

            over = new DrawablePath(*path);
            over->setStrokeFill(FillType(Colours::white));

            down = new DrawablePath(*path);
            down->setFill(FillType(Colours::red));

            setImages(path, over, down);
        }
        ~ControllerButton() {
            delete shape;
            delete path;
            delete over;
            delete down;
        }

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControllerButton)
};