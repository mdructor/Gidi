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
#include "AppSettings.h"

// Component to edit the global app settings
class OptionsComponent : public Component 
{
    private:
        Label      lblMapDirectory;
        TextEditor txtMapDirectory;
        TextButton btnMapDirectory;
        TextButton     btnCancel;
        TextButton     btnSave;

        Label lblMidiChannel;
        Slider sldrMidiChannel;

        Label lblVirtualMidi;
        TextEditor txtVirtualMidi;
        TextButton btnVirtualMidi;
        

        void resized() override;
        void paint(Graphics& g) override;

        // Button action handlers
        void onButtonCancel();
        void onButtonSave();
        void onMapDirectory();
        void onButtonMidiCreate();


    public:
        OptionsComponent();
        ~OptionsComponent();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OptionsComponent)
};