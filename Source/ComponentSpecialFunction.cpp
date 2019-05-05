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
    }
}