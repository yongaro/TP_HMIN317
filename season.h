#ifndef SEASON_H
#define SEASON_H

#include <string>
#include "particle.h"

// todo : singleton et 4 saisons des attribut statics
class Season {
public:
    Season();
    ~Season();

    static Season winter();
    static Season summer();
    static Season automn();
    static Season spring();

    void setName(std::string);
    std::string getName() const;

    Particle* getParticle();
private:
    std::string name;
    Particle particle;

};

#endif // SEASON_H
