#include "particle.h"
#include <string>
#include <glm/vec3.hpp>
#include "trianglewindow.h"
#include <iostream>
#include <GL/gl.h>
Particle::Particle() {
    position.x = 0;
    position.y = 0;
    position.z = 0;

    size = 0;
    setColor(255,255,255);
    setName("undefined");
}

Particle::~Particle() {}

void Particle::setName(std::string newName) {
    name = newName;
}

std::string Particle::getName() const {
    return name;
}

void Particle::setSize(unsigned char newSize) {
    size = newSize;
}

unsigned char Particle::getSize() const {
    return size;
}

void Particle::setColor(unsigned char r, unsigned char g, unsigned char b) {
    color.r = r < 255 ? r : 255;
    color.g = g < 255 ? g : 255;
    color.b = b < 255 ? b : 255;
}

struct Color Particle::getColor() const {
    return color;
}

void Particle::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

glm::vec3 Particle::getPosition() const {
    return position;
}

void Particle::setLifeTime(unsigned int nbFrame) {
    lifeTime = nbFrame;
}

unsigned int Particle::getLifeTime() const {
    return lifeTime;
}

void Particle::move () {
    setPosition(position.x, position.y - 0.05f, position.z);
}

bool Particle::live(TriangleWindow* ctx) {
    if (lifeTime > 0) {
        if (ctx->isOverTextureHeight(position.x, position.y, position.z)) {
            move();
        }
        return ((--lifeTime) > 0);
    } else {
        std::cerr << "une particule morte essaie de vivre" << std::endl;
        return false;
    }
}

void Particle::clear() {
    position.x = 0;
    position.y = 0;
    position.z = 0;

    size = 0;
    setColor(255,255,255);
    setName("undefined");
}

void Particle::copy(Particle* p) {
    position = p->position;
    lifeTime = p->lifeTime;
    size = p->size;
    color = p->color;
    setName(p->getName());
}
