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

#include "NoteParser.h"

int parseNote(String note) 
{

    // Also handle parsing Special Button Functions here. probably a bad idea but can 
    // fix it later
    // TODO:: ME
    if (note == "OctaveUp") {
        return (int) ComponentSpecialFunction::OctaveUp; 
    }
    else if(note == "OctaveDown") {
        return (int) ComponentSpecialFunction::OctaveDown;
    }
    else if (note == "PitchUp") {
        return (int) ComponentSpecialFunction::PitchUp;
    }
    else if (note == "PitchDown") {
        return (int) ComponentSpecialFunction::PitchDown;
    }
    else if (note == "Velocity") {
        return (int) ComponentSpecialFunction::Velocity;
    }
    else if (note == "PitchBend") {
        return (int) ComponentSpecialFunction::PitchBend;
    }
    else if (note == "Sustain") {
        return (int) ComponentSpecialFunction::Sustain;
    }

    int result = 0; 
    note = note.trim();
    if (note.length() < 2 || note.length() > 3) { // invalid note string
        return -1;
    } 
    note = note.toLowerCase(); // can at least be NOT case-sensitive

    if (note == "a0") { // handle the midi 0 notes 
        return 21;
    }
    else if (note == "a#0" || note == "bb0") {
        return 22;
    }
    else if (note == "b0") {
        return 23;
    }
    else if (note == "c8") {
        return 108;
    }

    auto first = note[0];
    if (first < 'a' || first > 'g') { // first char must be a note between a - g
        return -1;
    }
    switch (first) {
        case 'a': result = 33; break;
        case 'b': result = 35; break;
        case 'c': result = 24; break;
        case 'd': result = 26; break;
        case 'e': result = 28; break;
        case 'f': result = 29; break;
        case 'g': result = 31; break;
    }
    auto second = note[1];
    if (note.length() == 2) {
        if (second < '1' || second > '8') {
            return -1;
        }
        else {
            result += (12 * (second - '1'));
        }
    }
    else {
        if (second == 'b') {
            --result;
        }
        else if (second == '#') {
            ++result;
        }
        else {
            return -1;
        }
        auto third = note[2];
        if (third < '1' || third > '7') {
            return -1;
        }
        else {
            result += (12 * (third - '1'));
        }
    }
    return result;
}