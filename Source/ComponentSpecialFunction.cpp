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

#include "ComponentSpecialFunction.h"

String ComponentSpecialFunctionToString(ComponentSpecialFunction csf) 
{
    switch (csf) {
        case ComponentSpecialFunction::PitchUp: return "Pitch Up";
        case ComponentSpecialFunction::PitchDown: return "Pitch Down";
        case ComponentSpecialFunction::OctaveUp: return "Octave Up";
        case ComponentSpecialFunction::OctaveDown: return "Octave Down";
        case ComponentSpecialFunction::Velocity: return "Velocity";
        case ComponentSpecialFunction::PitchBend: return "Pitch Bend";
        case ComponentSpecialFunction::Sustain: return "Sustain";
    }
}