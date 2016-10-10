#ifndef POOL_H
#define POOL_H


#include <vector>
#include <list>
#include <algorithm>


template <class Ressource>
class Pool {
public:
    Pool<Ressource>();
    Pool<Ressource>(unsigned int);
    virtual ~Pool();

    // Management : Client call
    Ressource* ask();
    void recycle(Ressource*);
    std::list<Ressource*> avaibles;
    std::list<Ressource*> used;

private:
    std::vector<Ressource*> data;
//    std::list<Ressource*> avaibles;
//    std::list<Ressource*> used;

    // On remet à l'état d' "usine" la ressource
    void clear(Ressource*);
};
/*
template <class Ressource>
Pool<Ressource>::Pool() {}

template <class Ressource>
Pool<Ressource>::Pool(unsigned int size) {
    for (int i=0; i < size; ++i) {
        avaibles.push_back(Ressource());
    }
}

template <class Ressource>
static bool deleteElt(Ressource* ressource) {
    //delete ressource;
    return true;
}

template <class Ressource>
Pool<Ressource>::~Pool() {
    avaibles.remove_if(deleteElt);
    used.remove_if(deleteElt);
}

// Management : Client call
template <class Ressource>
Ressource Pool<Ressource>::ask() {
    Ressource asked;

    if (!avaibles.empty()) {
         asked = avaibles.back();
         avaibles.pop_back();
         used.push_back(asked);
    }

    return asked;
}

template <class Ressource>
void Pool<Ressource>::recycle(Ressource ressource) {
    // debug mode : tester si le ptr est dans used
    if (ressource != NULL) {
       clear(ressource);
       avaibles.push_back(ressource);

       // @TODO : O(n) -> O(1)
       used.remove(ressource);
    }
}

template <class Ressource>
void Pool<Ressource>::clear(Ressource ressource) {
    // debug mode : tester si le ptr est dans used
    if (ressource != NULL) {
       ressource.clear();
    }
}

*/
#endif // POOL_H
