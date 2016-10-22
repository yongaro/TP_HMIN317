#include "season.h"
#include <string>
#include<iostream>
// Initialisation des attributs statiques
bool Season::isInitialized = false;


Season Season::staticAutomn = Season();
Season Season::staticWinter = Season();
Season Season::staticSpring = Season();
Season Season::staticSummer = Season();

Season::Season() {
    name = "foo";
    //initSeasons();
}

Season::~Season() {}

void Season::initWinter () {
    std::cout << "initWinter" << std::endl;
    staticWinter.setName("Winter");
    staticWinter.particle.setColor(255,255,255);
    staticWinter.particle.setLifeTime(10000);
    staticWinter.particle.setName("Neige");
    staticWinter.particle.setSize(0.05f);
    staticWinter.particle.setPosition(0,0,0);
    staticWinter.particle.setWeight(0.05f);

    std::cout << "endWinter" << std::endl;
}

Season& Season::winter() {
    std::cout<<isInitialized<<std::endl;
    if (!isInitialized) {
        initSeasons();
    }
    return staticWinter;
}

void Season::initSeasons() {
    std::cout << "initialize" << std::endl;
    initWinter();
    initAutomn();
    initSpring();
    initSummer();
    isInitialized = true;
}

void Season::initSummer() {
    staticSummer.setName("Summer");
    staticSummer.particle.setSize(0.0f);

}

Season& Season::summer() {
    if (!isInitialized) {
        initSeasons();
    }
    return staticSummer;
}

void Season::initSpring() {

    staticSpring.setName("Spring");
    staticSpring.particle.setSize(0.0f);

}

Season& Season::spring() {
    if (!isInitialized) {
        initSeasons();
    }
    return staticSpring;
}

void Season::initAutomn() {
     Season::staticAutomn.setName("Automn");

    staticAutomn.particle.setColor(0.3, 0.4, 255);
    staticAutomn.particle.setLifeTime(180);
    staticAutomn.particle.setName("Pluie");
    staticAutomn.particle.setSize(0.005f);
    staticAutomn.particle.setPosition(0,0,0);
    staticAutomn.particle.setWeight(0.2f);

}

Season& Season::automn() {
    if (!isInitialized) {
        initSeasons();
    }
    return staticAutomn;
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


bool Season::gotParticle() const {
    return particle.getLifeTime() > 0;
}
