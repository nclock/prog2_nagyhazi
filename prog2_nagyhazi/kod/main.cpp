#include "toronyhaz.h"
#include <iostream>
#include <exception>

/**
 * \file main.cpp
 * \brief A futattást, és a felhasználói bemenetek kezelését és ellenőrzését végzi.
 * \details Első lépésben megvizsgálja az argumentumok számát, aztán megpróbálja átalakítani az érkező stringet intté,
 * ha nem sikerül, kivételt dob. \n Ha sikerül, akkor a toronyház objektumot létrehozza, és elindítja a liftet.
 * Addig írja ki a lift állapotát, illetve mozgatja azt, amíg az összes utas el nem érte a célállomását, vagyis a bekért
 * adatokból felépített vektorok üresek.
 * \param argc argumentumok száma
 * \param argv toronyház emeleteinek száma
 * \param N toronyház emeleteinek száma
 * \param toronyhaz a toronyház objektuma
 * \exception std::invalid_argument ha nem megfelelő számú argumentum érkezik, vagy nem egész számú emeletszám
 * \return int
*/
int main(int argc, char* argv[]) {
    try {

        if( argc != 2 ) {
            throw std::invalid_argument("nem megfelelő számú argumentum");
        }
        
        int N;
        try {
            N = std::stoi(argv[1]);
            if( N <= 0 ) {
            throw std::invalid_argument("nem létező emeletszámú ház");
            }
        }
        catch ( const std::invalid_argument& ) { //ha már átalakítani sem sikerült
            throw std::invalid_argument("nem egész számú emletszám");
        }

        Toronyhaz toronyhaz(N);
        toronyhaz.startLift(); // azért így, és nem getLift(), mert az konstanst ad vissza
                                // --> nem lehet módosítani a lift tagváltozóit

        while ( toronyhaz.getLift().getFloorsUp().size() > 0 || toronyhaz.getLift().getFloorsDown().size() > 0 ) {
            toronyhaz.displayLiftState();
            toronyhaz.move();
        }
    }
    
    catch (const std::exception& e) {
        std::cerr << "hiba: " << e.what() << std::endl;
    }
    
    return 0;
}