#include "toronyhaz.h"

/**
 * \file toronyhaz.cpp
 * @brief A toronyház osztály implementációja.
*/

int Toronyhaz::floorHeight = 3;

void Toronyhaz::displayLiftState() const {
            getLift().print();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int Toronyhaz::getFloorHeight() { return floorHeight; }

const int Toronyhaz::getFloorNum() const { return floorNum; }

const Lift& Toronyhaz::getLift() const { return lift; }

void Toronyhaz::startLift() { lift.initLift(getFloorNum()); }

void Toronyhaz::move() { lift.move(); }