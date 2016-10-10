#include "pool.h"

#include <vector>
#include <list>
#include <algorithm>
#include "trianglewindow.h"
#include "particle.h"
#include <iostream>

#include <assert.h>

template <class Ressource>
Pool<Ressource>::Pool() {
    data.clear();
    assert(data.size() == 0);
}

template <class Ressource>
Pool<Ressource>::Pool(unsigned int size) {
//#pragma omp parallel
    //{
        for (int i=0; i < size; ++i) {
            Ressource* ressource = new Ressource();
            data.push_back(ressource);
            avaibles.push_back(ressource);
        }
        //std::cout << avaibles.size() << std::endl;
    //}
}

template <class Ressource>
static bool deleteElt(Ressource* ressource) {
    delete ressource;
    return true;
}

template <class Ressource>
Pool<Ressource>::~Pool() {
    //std::cout << "DELETE" << std::endl;

    //#pragma omp parallel
    //{
        avaibles.clear();
        used.clear();

        const unsigned int size = data.size();
        for (int i=0; i < size; ++i) {
            Ressource *r = data.at(i);
            data.at(i) = NULL;
            delete r;
        }

        data.clear();
    //}
}

// Management : Client call
template <class Ressource>
Ressource* Pool<Ressource>::ask() {
    //std::cout << "TEST - ASK " << std::endl;
    Ressource* asked = NULL;

    //const unsigned int sizeUsed = used.size();
    //const unsigned int sizeAvaible = avaibles.size();

    if (!avaibles.empty()) {
         asked = avaibles.back();
         avaibles.pop_back();
         used.push_back(asked);
    } else {
        // On met une nouvelle ressource en circulation
        Ressource* ressource = new Ressource();
        data.push_back(ressource);
        asked = ressource ;
        used.push_back(ressource);
    }

    /*
    // DEBUG
    std::cout << "debug (used size): " << sizeUsed << " : " << used.size() << std::endl;
    std::cout << "debug (avaible size): " << sizeAvaible << " : " << avaibles.size() << std::endl;

    assert(asked != NULL);
    assert(sizeUsed < used.size());
    */

    return asked;
}

template <class Ressource>
void Pool<Ressource>::recycle(Ressource* ressource) {
    // debug
    //std::cout << "TEST - RECYCLE" << std::endl;
    //assert(ressource != NULL);


    //const unsigned int sizeAvaible = avaibles.size();
    //const unsigned int sizeUsed = used.size();
    //#pragma omp parallel
    //{
        if (ressource != NULL) {
           // @TODO : O(n) -> O(1)
           used.remove(ressource);
           clear(ressource);
           avaibles.push_back(ressource);


           //std::cout << "debug (used size): " << sizeUsed << " : " << used.size() << std::endl;
           //std::cout << "debug (avaible size): " << sizeAvaible << " : " << avaibles.size() << std::endl;
        }
    //}
    //assert(sizeAvaible < avaibles.size());
    //assert(sizeUsed > used.size());
}

template <class Ressource>
void Pool<Ressource>::clear(Ressource* ressource) {
    // debug
    //std::cout << "test clear ressource ... ";
    //assert(ressource != NULL);
    if (ressource != NULL) {
       ressource->clear();
    }

    //std::cout << "OK !" << std::endl;
}

template class Pool<TriangleWindow>;
template class Pool<Particle>;


