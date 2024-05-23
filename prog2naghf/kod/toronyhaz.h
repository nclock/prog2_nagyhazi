#pragma once
#include "lift.h"
#include <thread>
#include <chrono>

/**
 * @file toronyhaz.h
 * @brief A toronyház class deklarációja.
 
*/

/**
 * @class Toronyhaz
 * @brief A Toronyhaz osztály reprezentál egy toronyházat.
 * *\param floorHeight az emeletek magassága [m]
 * \param floorNum az emeletek száma
 * \param lift a lift objektuma
 * \return int
 */
class Toronyhaz {

private:
    static int floorHeight; // emeletek magassága [m]
    const int floorNum; //emeletek száma
    Lift lift;

public:

    /**
     * @brief Konstruktor. Inicializálja a toronyház emeleteinek számát és a lift objektumot.
     * @param N emeletek száma
    */
    Toronyhaz(const int N) : floorNum(N) {}

    /**
    * @brief Visszaadja a toronyház emeletmagasságát.
    * @param floorHeight emelet magassága
    * @return int
    */
    static int getFloorHeight();
    
    /**
     * @brief Visszaadja a toronyház emeleteinek számát.
     * @param floorNum emeletek száma
     * @return const int
    */
    const int getFloorNum() const;

    /**
    * @brief Visszaadja a toronyház liftjét.
    * @param lift tagváltozó
    * @return const Lift&
    */
    const Lift& getLift() const;

    /**
    * @brief Inicializálja a lift tagváltozóját.
    * @param lift tagváltozó
    * @return const Lift&
    */
    void startLift();

    /**
    * @brief Elindítja a liftet.
    * @param lift tagváltozó
    * @return void
    */
    void move();

    /**
     * @brief Megjeleníti a lift fél másodpercenkénti állapotát.
    * @param lift tagváltozó
    * @return void
    */
    void displayLiftState() const;
    
};

