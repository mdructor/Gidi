#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdexcept>
#include "GidiMap.h"
#include "NoteParser.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class GidiMapParser {

    private:
        const static StringArray searchTags;

    public:
        static GidiMap loadGidiMap(File mapFile);

};

class GidiMapParsingException : public std::runtime_error {

    public:
        GidiMapParsingException(char const* const message) throw()
            : std::runtime_error(message)
        {

        }

};