#pragma once
#include <vector>
#include <iostream>

enum Direction {
    UP = 1,
    DOWN = -1,
    STOP = 0
};

/**
 * @file lift.h
 * @brief A lift class deklarációja.
 * 
*/

/**
 * @class Lift
 * @brief A Lift osztály reprezentál egy liftet.
 * Az osztály felelős a lift mozgatásáért és állapotának kezeléséért.
 * Tartalmazza a lift aktuális emeletét, a felfelé és lefelé hívott emeleteket,
 * a lift irányát, sebességét, ajtajának állapotát, valamint a mozgás és ajtó
 * időzítéseket.
 * @param time indulástól eltelt idő [s]
 * @param currentFloor aktuális emelet (0 - N)
 * @param floorsUp felfelé hívott emeletek vektora
 * @param floorsDown lefelé hívott emeletek vektora
 * @param direction a lift iránya [UP, DOWN, STOP]
 * @param departureFloor indulási emelet
 * @param acceleration gyorsulás (konstans 1 [m/s^2])
 * @param maxVelocity maximális sebesség (konstans 5 [m/s])
 * @param velocity aktuális sebesség
 * @param isDoorOpen ajtó állapota (nyitva/zárva)
 * @param floorTime emelet megtételének ideje [s]
 * @param doorTime ajtónyitás és zárás ideje [s]
 * @return int
 */
class Lift {

private:
    double time;
    int currentFloor;
    std::vector<int> floorsUp; // jelenglegi pozihoz képest a külső gombot attól följebb hívták
    std::vector<int> floorsDown; // jelenglegi pozihoz képest a külső gombot attól lejjebb hívták
    Direction direction; // felfelé / lefelé / álló helyzet
    int departureFloor;

    const int acceleration;                 
    const double maxVelocity;
    double velocity; // aktuális sebesség -5 - 5 [m/s] irányfüggően
    bool isDoorOpen; // ha nyitva van, akkor nem megy

    double floorTime; // emelet megtételének ideje [s]
    const int doorTime; // ajtó nyitás, zárás és beszállás ideje [s]
    
    /**
     * @brief Kiszámolja, mekkora sebességgel mozogjon a lift.
     * A megadosott gyorsulás és a még megteendő út alapján határozza meg a sebességet. Az teljes út feléig gyorsít, majd a második felében lassít.
     * Amennyiben a lift iránya STOP, a sebesség 0. A megválasztott sebesség mellett kiszámolja, mennyi idő alatt tesz meg a lift egy emeletet, és a
     * várakozási időhöz adja az értéket. Ezt kívülről nem lehet módosítani, ezért privát tagfüggvény.
     * @return void
     */
    void setVelocity();

public:
     /**
     * @brief Konstruktor. Inicializálja a lift tagváltozóit.
    */
    Lift()
     : time(0)
     , currentFloor(0)
     , direction(STOP)
     , departureFloor(0)
     , acceleration(1)
     , maxVelocity(5)
     , velocity(0.0)
     , isDoorOpen(false)
     , floorTime(0.0)
     , doorTime(6) {};

    /*mivel beépitett típusokat használok, ezért azok destruktorai és copy konstruktorai jól működnek nem kell külön írni*/
    /**
     * @brief Destruktor.
     * Az alapértelmezett implementációval rendelkezik.
     */
    ~Lift() = default;

    /**
     * @brief Copy kostruktor.
     * Az alapértelmezett implementációval rendelkezik.
     */
    Lift(const Lift&) = default;

    /**
     * @brief Betölti a megfelelő vektorba az állomásokat.
     * Ha az indulási emelet nagyobb, mint a cél, akkor a lefelé listába kerül, egyébként a felfelé listába. Miután betöltésre került a listába, azt sorrendbe rendezi,
     * így a lehető leghatékonyabban tud majd közlekedni.
     * @param start indulási emelet
     * @param dest célállomás
     * @return void
     */
    void setTargetFloor(int start, int dest);

    /**
     * @brief Beállítja a lift irányát.
     * @param dir az új irány.
     * @return void
     */
    void setDirection(int dir);
        
    /**
     * @brief Nyitja vagy zárja az ajtót, ha a lift iránya STOP. Ezen felül van egy ideje, míg kinyíilik / bezárul az ajtó, ami hozzáadódik az összesen eltelt időhoz.
     * @return void
     */
    void setDoor();

    /**
     * @brief Visszaadja a lift aktuális emeletét.
     * @return int
     */
    int getCurrentFloor() const;

    /**
     * @brief Visszaadja a felfelé hívott emeletek vektorát.
     * @return const std::vector<int>&
     */
    const std::vector<int>& getFloorsUp() const;

    /**
     * @brief Visszaadja a lefelé hívott emeletek vektorát.
     * @return const std::vector<int>&
     */
    const std::vector<int>& getFloorsDown() const;

    /**
     * @brief Visszaadja a lift irányát.
     * @return Direction
     */
    Direction getDirection() const;

    /**
     * @brief Visszaadja a lift sebességét.
     * @return double
     */
    double getVelocity() const;

    /**
     * @brief Visszaadja, hogy az ajtó nyitva, vagy zárva van.
     * @return bool
     */
    bool getIsDoorOpen() const;

    /**
     * @brief Inicializálja a liftet a beérkező adatok alapján.
     * Futtatási argumentumként kapja meg a toronyház emeleteinek számát, majd felhasználótól bekéri, hogy honnan és hova szeretnének utazni. Ezen adatok érvényességét
     * ellenőrzi és meghívja rájuk a setTargetFloor() függvényt. Amennyiben szeretnének még utazni, újabb adatokat kér be, és újabb setTargetFloor() hívás történik.
     * A ciklusból akkor lép ki, ha már nem szeretnének máshová utazni.
     * @param floor a toronyház emeletszáma
     */
    void initLift(int floor);

    /**
     * @brief Mozgatja a liftet.
     * Emeletenként mozgatja a liftet a megfelelő irányba. Ha a lift iránya STOP, akkor nem mozog. A mozgás során a sebességét is beállítja. Ha már nincs több
     * úticél, akkor is megáll. Ha elér egy úticélt kinyitja az ajtót, bezárja, törli a vektorból az összes olyan tagot, ami megegyezik az aktuális emelettel és növeli
     * az összes időt (hisz van ideje, míg az utasok ki- is beszállnak).
     */
    void move();

    /**
     * @brief Kiírja a lift állapotát a konzolra.
     * Az aktuális emeletet, sebességet, irányt, eltelt időt és a megnyomott gombokat írja ki, utóbbiakat már sorba rendezve.
     */
    void print() const;
};