#include "pool.h"

#include <list>
#include <algorithm>
#include "trianglewindow.h"
#include "particle.h"
#include <iostream>

template <class Ressource>
Pool<Ressource>::Pool() {}

template <class Ressource>
Pool<Ressource>::Pool(unsigned int size) {
    for (int i=0; i < size; ++i) {
        avaibles.push_back(new Ressource());
    }
    std::cout << avaibles.size() << std::endl;
}

template <class Ressource>
static bool deleteElt(Ressource* ressource) {
    //delete ressource;
    return true;
}

template <class Ressource>
Pool<Ressource>::~Pool() {
    //avaibles->remove_if(deleteElt);
    //used->remove_if(deleteElt);
}

// Management : Client call
template <class Ressource>
Ressource* Pool<Ressource>::ask() {
    Ressource* asked = NULL;

    if (!avaibles.empty()) {
         asked = avaibles.back();
         avaibles.pop_back();
         used.push_back(asked);
    } else {
        asked = new Ressource();
        used.push_back(asked);
    }

    return asked;
}

template <class Ressource>
void Pool<Ressource>::recycle(Ressource* ressource) {
    // debug mode : tester si le ptr est dans used
    if (ressource != NULL) {
       clear(ressource);
       avaibles.push_back(ressource);

       // @TODO : O(n) -> O(1)
       //used.remove(ressource);
    }
}

template <class Ressource>
void Pool<Ressource>::clear(Ressource* ressource) {
    // debug mode : tester si le ptr est dans used
    if (ressource != NULL) {
       ressource->clear();
    }
}

template class Pool<TriangleWindow>;
template class Pool<Particle>;


