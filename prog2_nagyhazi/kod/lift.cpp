#include "lift.h"
#include "toronyhaz.h"
#include <algorithm> //sort fgv
#include <math.h>
#include <typeinfo> // kivétel kezeléshez

/**
 * \file lift.cpp
 * @brief A lift osztály implementációja.
 
*/

/*----------------------------------------------------------------SETTEREK-----------------------------------------------------------------*/

void Lift::setDirection(int dir) {
    if (dir == UP) {
        direction = UP;
    } else if (dir == DOWN) {
        direction = DOWN;
    } else {
        direction = STOP;
    }
}

void Lift::setDoor() {
    isDoorOpen = !isDoorOpen;
    time += doorTime;
}

void Lift::setTargetFloor(int start, int dest) { // ezt a gombot nyomja meg a felhasználó (érkezik a standard inputon)
    if( start == dest ) {
        return;
    }

    else if ( start < dest ) { //fel irányban hívják
                    floorsUp.push_back(start);
                    floorsUp.push_back(dest);
                    std::sort(floorsUp.begin(), floorsUp.end());   
    }

    else { // le irányban hívják
                    floorsDown.push_back(start);
                    floorsDown.push_back(dest);
                    std::sort(floorsDown.begin(), floorsDown.end(), std::greater<int>()); // csökkenő sorrendbe rendezi
    }
}

/*kényelmes sebesség és gyorsulás a 5 m/s és 1 m/s^2 --> 5 s kell, hogy megálljon /felgyorsuljon 0-ról maxra
  egy emelet 3 méter --> egy emeletet 3/5 s alatt tesz meg max sebességgel--> 5/(3/5) = 8,33 emelet kell, hogy maxról megálljon
    v = sqrt(v0 + 2*a*s)

*/
void Lift::setVelocity() {

    if( direction == STOP)
        velocity = 0;

    else if(direction == UP) {
        int distance = floorsUp[0] - departureFloor;
        if ( currentFloor <= departureFloor + (double)(distance / 2) && velocity < maxVelocity )
        {
            velocity += sqrt(2.0 * acceleration * (floorsUp[0] - currentFloor));
            if (velocity > maxVelocity)
                velocity = maxVelocity;
        }
        
        else if ( currentFloor < departureFloor + (double)distance / 2)
            velocity = maxVelocity;
    
        else
            velocity -= sqrt(2.0 * acceleration * (floorsUp[0] - currentFloor));
      
    }
    else if(direction == DOWN) {
        int distance = departureFloor - floorsDown[0];
        if ( currentFloor > departureFloor - (double)(distance / 2) && velocity < maxVelocity ) {
            velocity -= sqrt(2 * acceleration * (currentFloor - floorsDown[0]));
            if (velocity < -(maxVelocity));
                velocity = -(maxVelocity);
        }
        else if ( currentFloor > departureFloor - (double)distance / 2)
            velocity = -maxVelocity;
    
        else
            velocity += sqrt(2 * acceleration * (currentFloor - floorsDown[0]));
    }

    if ( velocity != 0)
        floorTime = abs(Toronyhaz::getFloorHeight() / velocity);
}  

/*----------------------------------------------------------------GETTEREK-----------------------------------------------------------------*/

int Lift::getCurrentFloor() const { return currentFloor; }

const std::vector<int>& Lift::getFloorsUp() const {return floorsUp; }

const std::vector<int>& Lift::getFloorsDown() const { return floorsDown; }

Direction Lift::getDirection() const { return direction; }

double Lift::getVelocity() const { return velocity; }

bool Lift::getIsDoorOpen() const { return isDoorOpen; }

/*---------------------------------------------------------------EGYÉB--------------------------------------------------------------------*/

void Lift::move() {
    if ( floorsUp.size() == 0 && floorsDown.size() == 0 )
    {
        Lift::setDirection(STOP);
        Lift::setVelocity();
    }

    if ( floorsUp.size() > 0 && isDoorOpen == false ) {
        if( currentFloor == floorsUp[0]) {
            departureFloor = currentFloor;
            Lift::setDirection(STOP);
            Lift::setVelocity();
            Lift::setDoor(); // nyit
            Lift::setDoor(); // zár
            time += 8; //8 s alatt szállnak be az utasok
            /*ha több utas száll itt be, akkor mindet egyszerre törölje*/
            floorsUp.erase(std::remove(floorsUp.begin(), floorsUp.end(), currentFloor), floorsUp.end());
        }
        
        else {
            Lift::setDirection(UP);
            Lift::setVelocity();

            if ( currentFloor < floorsUp[0] ) {
                currentFloor++;
            }
            else if ( currentFloor > floorsUp[0] ) {
                currentFloor--;
            }
        }
    }
 
    else if ( floorsDown.size() > 0 && isDoorOpen == false ) {

        if (currentFloor == floorsDown[0]) {
            departureFloor = currentFloor;
            Lift::setDirection(STOP);
            Lift::setVelocity();
            Lift::setDoor(); // nyit
            Lift::setDoor(); // zár
            time += 8; //8 s alatt szállnak be az utasok
            /*ha több utas száll itt be, akkor mindet egyszerre törölje*/
            floorsDown.erase(std::remove(floorsDown.begin(), floorsDown.end(), currentFloor), floorsDown.end());
        }

        else {
            Lift::setDirection(DOWN);
            Lift::setVelocity();

            if ( currentFloor < floorsDown[0] ) {
                currentFloor++;
            }
            else if ( currentFloor > floorsDown[0] ) {
                currentFloor--;
            } 
        }
    }

    
    time += floorTime;
}

void Lift::initLift(int N) {

    int start, dest;
    while( true ) {
        std::cout << "Honnan hívják a liftet? ";
            std::cin >> start;
            if( !std::cin || start < 0 || start > N ) { // ha nem egész számot ad meg, nem tud beolvasni sem
                std::cin.clear(); // hibás állapotot töröl
                throw std::invalid_argument("nem egész számú /negatív / túl nagy értékű indulási emelet");
            }
    
            std::cout << "Hova szeretnének menni? "; //Söci típusú lift
            std::cin >> dest;
            if( !std::cin || dest < 0 || dest > N ) {
                std::cin.clear();
                throw std::invalid_argument("nem egész számú /negatív / túl nagy értékű célállomás");
            }

            Lift::setTargetFloor(start, dest);

            std::cout << "Várnak még lift hívásra? (igen: 1, nem: 0) ";
            int answer;
            std::cin >> answer;
            if( !std::cin ) {
                std::cin.clear();
                throw std::invalid_argument("nem egész formátumú utazási szándék");
            }
            else if ( answer < 0 || answer > 1 ) {
                throw std::invalid_argument("nem '1 / 0' formátumú utazási szándek");
            }

            if (answer == 0) {
                return;
            }

    }
}

void Lift::print() const{
    std::cout << "Jelenlegi emelet: " << currentFloor << std::endl;
    std::cout << "Sebesség: " << velocity << " m/s" << std::endl;
    std::cout << "Közlekedési irány: " << direction << std::endl;
    /*mivel egyszerre történik a gombok bekérése, így az aktuálisan kiírt time mutatja, hogy mennyi ideje van megnyomva a külső gomb*/
    std::cout << "Az eltelt idő: " << time << "s" << std::endl;

    std::cout << "Felfelé hívott emeletek: ";
    for (int i = 0; i < floorsUp.size(); i++) {
        std::cout << floorsUp[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Lefelé hívott emeletek: ";
    for (int i = 0; i < floorsDown.size(); i++) {
        std::cout << floorsDown[i] << " ";
    }
    std::cout << std::endl;
}