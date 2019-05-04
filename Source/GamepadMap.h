#pragma once

#include "ComponentType.h"
#include <unordered_map>


template<typename T>
class GamepadMap : public std::unordered_map<ComponentType, T> {

    public: 
        GamepadMap() : std::unordered_map<ComponentType, T>() { }

};