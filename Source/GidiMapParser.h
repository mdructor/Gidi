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
        static Array<GidiMap> loadMapsFromDir(File dir);

};

class GidiMapParsingException : public std::runtime_error {

    public:
        GidiMapParsingException(char const* const message) throw()
            : std::runtime_error(message)
        {

        }

};