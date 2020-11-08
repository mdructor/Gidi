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

#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiProcessor.h"
#include "GidiMap.h"
#include "GidiMapParser.h"
#include "GamepadComponent.h"
#include "NewMidiDialog.h"
#include "AppSettings.h"
#include "OptionsComponent.h"
#include "ComponentSpecialFunction.h"
#include "NoteParser.h"

// Content of the main window -- holds control over the GidiProcessor

class MainComponent   : public Component, public ChangeListener, public Timer
{
    private:
        const int REFRESH_TIME = 2000; // time between combo box refreshes

        ComboBox    cbControllers; 
        ComboBox    cbMappings;
        ComboBox    cbMidiPorts;
        ImageButton btnSettings;
        ImageButton btnToggle;
        ImageComponent imgLogo;
        Image logoIcon = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile(AppSettings::getIconsDirectory()).getChildFile("logo.png"));
        Image settingsIcon = PNGImageFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(AppSettings::getIconsDirectory()).getChildFile("settings.png"));
        Image playIcon = PNGImageFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(AppSettings::getIconsDirectory()).getChildFile("play-button.png")); // TODO: MOVE THIS LOAD ELSEWHERE
        Image pauseIcon = PNGImageFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(AppSettings::getIconsDirectory()).getChildFile("pause-button.png"));
        Font fntTitle;
        Label lblTitle;
        Label       lblController;
        Label       lblMapping;
        Label       lblMidiPort;
        TextEditor txtMapInfo;
        Label  lblVelocity;
        Label  lblOctave;
        Label  lblPitch;
        Slider sldrVelocity;
        Slider sldrOctave;
        Slider sldrPitch;

        std::unique_ptr<MidiKeyboardComponent> midiVisual;
        std::shared_ptr<GamepadComponent> gamepadComponent;

        std::unique_ptr<GidiProcessor> processor;

        std::shared_ptr<MidiKeyboardState>      keyboardState;

        Array<GidiMap> loadedMaps;

        bool isProcessing = false;


        void refreshComboBoxes();
        void refreshMapComboBox();

        void toggle();

        void openOptionsDialog();

        void onSldrVelocityChange();
        void onSldrOctaveChange();
        void onSldrPitchChange();

        void onGamepadButtonStateChange(ControllerButton* src);

    public:
        MainComponent();
        ~MainComponent();

        void paint (Graphics& g) override;
        void resized() override;
        virtual void changeListenerCallback(ChangeBroadcaster* source) override;
        virtual void timerCallback() override;
        


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
