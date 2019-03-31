#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiLogger.h"

// https://wireframe.cc/7IqUMF

class ActiveView : public Component {

    private:
        TextEditor txtMapInfo;
        TextEditor txtLog;

        void resized() override {
            txtMapInfo.setBounds(300, 10, 195, 90);
            txtLog.setBounds(300, 110, 195, 180);
        }


    public:

        ActiveView() {
            txtMapInfo.setMultiLine(true);
            txtLog.setMultiLine(true);
            txtMapInfo.setReadOnly(true);
            txtLog.setReadOnly(true);

            addAndMakeVisible(txtMapInfo);
            addAndMakeVisible(txtLog);


            for (auto msg : GidiLogger::logQueue) {
                txtLog.setText(txtLog.getText() + "\n" + msg);
            }

            setSize(500, 300);
        }
        ~ActiveView() {}


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveView)
};