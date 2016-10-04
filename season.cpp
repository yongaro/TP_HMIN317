#include "season.h"
#include <string>

Season::Season() {
    name = "foo";
}

Season::~Season(){}


Season Season::winter(){
    Season winter;
    winter.setName("Winter");
    winter.particle.setColor(255,255,255);
    winter.particle.setLifeTime(10000);
    winter.particle.setName("Neige");
    winter.particle.setSize(0.000015f);
    winter.particle.setPosition(0,0,0);
    winter.particle.setWeight(0.05f);
    return winter;
}

Season Season::summer(){
    Season summer;
    summer.setName("Summer");
    return summer;
}

Season Season::spring(){
    Season spring;
    spring.setName("Spring");
    return spring;
}
Season Season::automn(){
    Season automn;
    automn.setName("Automn");


    automn.particle.setColor(180,180,200);
    automn.particle.setLifeTime(180);
    automn.particle.setName("Pluie");
    automn.particle.setSize(0.01);
    automn.particle.setPosition(0,0,0);
    automn.particle.setWeight(0.08f);
    return automn;
}


void Season::setName(std::string newName) {
    name = newName;
}

std::string Season::getName() const {
    return name;
}

Particle* Season::getParticle() {
    return &particle;
}
