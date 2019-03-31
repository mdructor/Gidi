#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GidiLogger : public ChangeListener {
    public:
        static StringArray logQueue;
        static ChangeBroadcaster listeners;
        static void logMsg(String msg);
        static void addListener(ChangeListener* listener);
        static void removeListener(ChangeListener* listener);
};