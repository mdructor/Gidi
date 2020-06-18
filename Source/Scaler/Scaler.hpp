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

#pragma once

#include <cmath>
#include <string>
#include <stdexcept>
#include <vector>

namespace Scaler
{

class Key
{
    public:
        enum class Type {A, B, C, D, E, F, G};

        Key(Type t = Type::C);

        Type getType();

        std::string toString();
    
    private:
        Type keyType;
};

class Accidental
{
    public:
        enum class Type {natural, flat, sharp, double_flat, double_sharp};

        Accidental(Type t = Type::natural);

        Type getType();

        std::string toString();
    
    private:
        Type accType;
};

class Note
{
    public:
        Note(Key k = Key(Key::Type::C), Accidental a = Accidental(Accidental::Type::natural), int o = 4);
        Note(std::string value);
        Note(int midiValue, bool useSharps = true);

        Key getKey();
        Accidental getAccidental();
        int getOctave();
        int getMidiValue();
        double getFrequency();

        std::string toString();

    private:
        Key key;
        Accidental accidental;
        int octave;

};

class Interval
{
    /*
        Only handles Simple Intervals atm (degree 1-8)
    */
    public:
        enum class Quality { perfect, major, minor, augmented, diminished };
        Interval(Quality q = Quality::perfect, int degree = 1);

        Quality getQuality();
        int getDegree();
        int getSemitones();
        Note getNoteFromRoot(Note root);

        std::string toString();

    private:
        Quality quality;
        int degree;

};


class Scale
{
    public:
        Scale(std::vector<Interval> inters);

        std::vector<Note> getNotesFromRoot(Note r);

    private:
        std::vector<Interval> intervals;

};


class Chord 
{
    public:
        Chord(std::vector<Interval> inters);
        
        std::vector<Note> getNotesFromRoot(Note r);

    private:
        std::vector<Interval> intervals;

};


namespace Intervals
{
    extern Interval P1;
    extern Interval m2;
    extern Interval M2;
    extern Interval m3;
    extern Interval M3;
    extern Interval P4;
    extern Interval A4;
    extern Interval P5;
    extern Interval m6;
    extern Interval M6;
    extern Interval m7;
    extern Interval M7;
    extern Interval P8;
}

namespace Scales
{
    extern Scale Major;
    extern Scale NaturalMinor;
    extern Scale MelodicMinor;
    extern Scale HarmonicMinor;
}

namespace Chords
{
    extern Chord Major;
    extern Chord Minor;
}

class NoteParsingException : public std::runtime_error
{
    public:
        NoteParsingException(char const* const message) throw() : std::runtime_error(message) {}
};

class InvalidIntervalException : public std::runtime_error
{
    public:
        InvalidIntervalException(char const* const message) throw() : std::runtime_error(message) {}
};




}