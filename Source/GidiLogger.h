#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/* DEPRECATED ATM - MAY REMOVE IN FUTURE */
/* CURRENTLY NOT USING THIS ANYWHERE */

class GidiLogger : public ChangeListener {
    public:
        static StringArray logQueue;
        static ChangeBroadcaster listeners;
        static void logMsg(String msg);
        static void addListener(ChangeListener* listener);
        static void removeListener(ChangeListener* listener);
};