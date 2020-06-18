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

// Scaler v0.0.1

#include "Scaler.hpp"

namespace Scaler {

Key::Key(Key::Type t) {
    keyType = t;
}

Key::Type Key::getType() {
    return keyType;
}

std::string Key::toString() {
    switch(keyType) {
        case Key::Type::A: return "A";
        case Key::Type::B: return "B";
        case Key::Type::C: return "C";
        case Key::Type::D: return "D";
        case Key::Type::E: return "E";
        case Key::Type::F: return "F";
        case Key::Type::G: return "G";
    }
    return "";
}

Accidental::Accidental(Accidental::Type t) {
    accType = t;
}

Accidental::Type Accidental::getType() {
    return accType;
}

std::string Accidental::toString() {
    switch (accType) {
        case Accidental::Type::natural: return "";
        case Accidental::Type::flat: return "b";
        case Accidental::Type::sharp: return "#";
        case Accidental::Type::double_flat: return "bb";
        case Accidental::Type::double_sharp: return "##";
    }
    return "";
}

Note::Note(Key k, Accidental a, int o) {
    key = k;
    accidental = a;
    octave = o;
}

Note::Note(std::string value) {
    std::string errorString = "Couldn't parse note: " + value;
    if (value.length() < 2 || value.length() > 4) {
        throw NoteParsingException(errorString.c_str());
    }

    // Deal with Key value
    auto fc = value.at(0);

    if (fc >= 'a' && fc <= 'g') {
        key = Key( (Key::Type) (fc - 'a')); 
    }
    else if(fc >= 'A' && fc <= 'G') {
        key = Key ( (Key::Type) (fc - 'A'));
    }
    else {
        throw NoteParsingException(errorString.c_str());
    }

    // Deal with Octave value
    auto lc = value.back();

    if (lc >= '0' && lc <= '9') {
        octave = lc - '0';
    }
    else {
        throw NoteParsingException(errorString.c_str());
    }

    // Deal with accidentals

    if (value.length() > 2) {
        auto fb = value.find_first_of('b');
        auto fs = value.find_first_of('#');
        if (fb == std::string::npos && fs == std::string::npos) {
            throw NoteParsingException(errorString.c_str());
        }
        else {
            if (value.length() > 3) {
                if (fb != std::string::npos) {
                    if (value.find_first_of('b', fb + 1) != std::string::npos) {
                        accidental = Accidental(Accidental::Type::double_flat);
                    }
                    else {
                        throw NoteParsingException(errorString.c_str());
                    }
                }
                else {
                    if (value.find_first_of('#', fs + 1) != std::string::npos) {
                        accidental = Accidental(Accidental::Type::double_sharp);

                    }
                    else {
                        throw NoteParsingException(errorString.c_str());
                    }
                }
            }
            else {
                if (fb != std::string::npos) {
                    accidental = Accidental(Accidental::Type::flat);
                }
                else {
                    accidental = Accidental(Accidental::Type::sharp);
                }
            }
        }
    }
    else {
        accidental = Accidental();
    }

}

Note::Note(int midiValue, bool useSharps) {
    std::string errorMessage = "Couldn't parse midi value to note: " + midiValue;
    if (midiValue > 127 || midiValue < 10) {
        throw NoteParsingException(errorMessage.c_str());
    }

    int r = midiValue % 12;

    if (r == 0) {
        key = Key(Key::Type::C); 
    }
    else if (r == 1) {
        if (useSharps) {
            key = Key(Key::Type::C); 
            accidental = Accidental(Accidental::Type::sharp);
        }
        else {
            key = Key(Key::Type::D);
            accidental = Accidental(Accidental::Type::flat);
        }
    }
    else if (r == 2) {
        key = Key(Key::Type::D); 
    }
    else if (r == 3) {
        if (useSharps) {
            key = Key(Key::Type::D); 
            accidental = Accidental(Accidental::Type::sharp);
        }
        else {
            key = Key(Key::Type::E);
            accidental = Accidental(Accidental::Type::flat);
        }
    }
    else if (r == 4) {
        key = Key(Key::Type::E); 
    }
    else if (r == 5) {
        key = Key(Key::Type::F);
    }
    else if (r == 6) {
        if (useSharps) {
            key = Key(Key::Type::F); 
            accidental = Accidental(Accidental::Type::sharp);
        }
        else {
            key = Key(Key::Type::G);
            accidental = Accidental(Accidental::Type::flat);
        }
    }
    else if (r == 7) {
        key = Key(Key::Type::G);
    }
    else if (r == 8) {
        if (useSharps) {
            key = Key(Key::Type::G); 
            accidental = Accidental(Accidental::Type::sharp);
        }
        else {
            key = Key(Key::Type::A);
            accidental = Accidental(Accidental::Type::flat);
        }
    }
    else if (r == 9) {
        key = Key(Key::Type::A);
    }
    else if (r == 10) {
        if (useSharps) {
            key = Key(Key::Type::A); 
            accidental = Accidental(Accidental::Type::sharp);
        }
        else {
            key = Key(Key::Type::B);
            accidental = Accidental(Accidental::Type::flat);
        }
    }
    else {
        key = Key(Key::Type::B);
    }

    octave = (midiValue - 12) / 12;

}

Key Note::getKey() {
    return key;
}

Accidental Note::getAccidental() {
    return accidental;
}

int Note::getOctave() {
    return octave;
}

int Note::getMidiValue() {
    int midiVal;
    switch (key.getType()) {
        case Key::Type::A: 
            midiVal = 21; 
            break;
        case Key::Type::B: 
            midiVal = 23;
            break;
        case Key::Type::C:
            midiVal = 12;
            break;
        case Key::Type::D:
            midiVal = 14;
            break;
        case Key::Type::E:
            midiVal = 16;
            break;
        case Key::Type::F:
            midiVal = 17;
            break;
        case Key::Type::G:
            midiVal = 19;
            break;
    }
    switch (accidental.getType()) {
        case Accidental::Type::sharp:
            ++midiVal;
            break;
        case Accidental::Type::flat:
            --midiVal;
            break;
        case Accidental::Type::double_sharp:
            midiVal += 2;
            break;
        case Accidental::Type::double_flat:
            midiVal -= 2;
            break;
        default: break;
    }
    midiVal += (octave * 12);
    return midiVal;
}

double Note::getFrequency() {
    return std::pow(2, (getMidiValue() - 69)/12.0) * 440;
}

Interval::Interval(Interval::Quality q, int d) {
    std::string errorMessage = "Invalid interval error.";
    quality = q;
    degree = d;
    // Check for Invalid Intervals
    if (quality == Quality::perfect) {
        if (degree != 1 && degree != 4 && degree != 5 && degree != 8) {
            throw InvalidIntervalException(errorMessage.c_str());
        }
    }
    else if (quality == Quality::major || quality == Quality::minor) {
        if (degree != 2 && degree != 3 && degree != 6 && degree != 7) {
            throw InvalidIntervalException(errorMessage.c_str());
        }
    }
    else if (quality == Quality::augmented) {
        if (degree < 2 || degree > 8) {
            throw InvalidIntervalException(errorMessage.c_str());
        }
    }
    else {
        if (degree < 1 || degree > 7) {
            throw InvalidIntervalException(errorMessage.c_str());
        }
    }
}

std::string Note::toString() {
    return key.toString() + accidental.toString() + std::to_string(octave);
}

Interval::Quality Interval::getQuality() {
    return quality;
}

int Interval::getDegree() {
    return degree;
}

int Interval::getSemitones() {
    auto s = toString();
    if (s == "P1" || s == "d2") {
        return 0;
    }
    else if(s == "m2" || s == "A1") {
        return 1;
    }
    else if(s == "M2" || s == "d3") {
        return 2;
    }
    else if(s == "m3" || s == "A2") {
        return 3;
    }
    else if(s == "M3" || s == "d4") {
        return 4;
    }
    else if(s == "P4" || s == "A3") {
        return 5;
    }
    else if(s == "d5" || s == "A4") {
        return 6;
    }
    else if(s == "P5" || s == "d6") {
        return 7;
    }
    else if(s == "m6" || s == "A5") {
        return 8;
    }
    else if(s == "M6" || s == "d7") {
        return 9;
    }
    else if(s == "m7" || s == "A6") {
        return 10;
    }
    else if(s == "M7" || s == "d8") {
        return 11;
    }
    else {
        return 12;
    }
}

Note Interval::getNoteFromRoot(Note root) {
    return Note(root.getMidiValue() + getSemitones());
}

std::string Interval::toString() {
    std::string res = "";
    switch(quality) {
        case Quality::perfect:
            res += "P";
            break;
        case Quality::minor:
            res += "m";
            break;
        case Quality::major:
            res += "M";
            break;
        case Quality::augmented:
            res += "A";
            break;
        case Quality::diminished:
            res += "d";
            break;
    }

    res += std::to_string(degree);
    return res;
}


Scale::Scale(std::vector<Interval> inters) {
    intervals = inters;
}

std::vector<Note> Scale::getNotesFromRoot(Note r) {
    std::vector<Note> notes;
    for (auto inter : intervals) {
        notes.push_back(inter.getNoteFromRoot(r));
    }
    return notes;
}



Chord::Chord(std::vector<Interval> inters) {
    intervals = inters;
}

std::vector<Note> Chord::getNotesFromRoot(Note r) {
    std::vector<Note> notes;
    for (auto inter : intervals) {
        notes.push_back(inter.getNoteFromRoot(r));
    }
    return notes;
}


namespace Intervals
{
    Interval P1(Interval::Quality::perfect, 1);
    Interval m2(Interval::Quality::minor, 2);
    Interval M2(Interval::Quality::major, 2);
    Interval m3(Interval::Quality::minor, 3);
    Interval M3(Interval::Quality::major, 3);
    Interval P4(Interval::Quality::perfect, 4);
    Interval A4(Interval::Quality::augmented, 4);
    Interval P5(Interval::Quality::perfect, 5);
    Interval m6(Interval::Quality::minor, 6);
    Interval M6(Interval::Quality::major, 6);
    Interval m7(Interval::Quality::minor, 7);
    Interval M7(Interval::Quality::major, 7);
    Interval P8(Interval::Quality::perfect, 8);
}

namespace Scales
{
    Scale Major( {Intervals::P1, Intervals::M2, Intervals::M3, Intervals::P4, Intervals::P5, Intervals::M6, Intervals::M7, Intervals::P8} );
    Scale NaturalMinor( {Intervals::P1, Intervals::M2, Intervals::m3, Intervals::P4, Intervals::P5, Intervals::m6, Intervals::m7, Intervals::P8} );
    Scale MelodicMinor( {Intervals::P1, Intervals::M2, Intervals::m3, Intervals::P4, Intervals::P5, Intervals::M6, Intervals::M7, Intervals::P8} );
    Scale HarmonicMinor( {Intervals::P1, Intervals::M2, Intervals::m3, Intervals::P4, Intervals::P5, Intervals::m6, Intervals::M7, Intervals::P8} );
}

namespace Chords
{
    Chord Major( {Intervals::P1, Intervals::M3, Intervals::P5});
    Chord Minor( {Intervals::P1, Intervals::m3, Intervals::P5});
}

}