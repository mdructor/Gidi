#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GidiLogger {
    public:
        static StringArray logQueue;

        static void log(String msg) {
            logQueue.add(msg);
        }
};