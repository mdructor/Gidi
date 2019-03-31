#include "GidiLogger.h"

StringArray GidiLogger::logQueue;
ChangeBroadcaster GidiLogger::listeners;

void GidiLogger::logMsg(String msg) {
    GidiLogger::logQueue.add(msg);
    listeners.sendChangeMessage();
}

void GidiLogger::addListener(ChangeListener* listener) {
    listeners.addChangeListener(listener);
}

void GidiLogger::removeListener(ChangeListener* listener) {
    listeners.removeChangeListener(listener);
}