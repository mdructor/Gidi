#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GidiLogger.h"

// https://wireframe.cc/7IqUMF

class ActiveView : public Component, public ChangeListener {

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

            GidiLogger::addListener(this);

            setSize(500, 300);
        }

        ~ActiveView() {
            GidiLogger::removeListener(this);
        }

        virtual void changeListenerCallback(ChangeBroadcaster* source) override {
            for (auto msg : GidiLogger::logQueue) {
                txtLog.setText(txtLog.getText() + "\n" + msg);
            }
            GidiLogger::logQueue.clear();
        }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ActiveView)
};