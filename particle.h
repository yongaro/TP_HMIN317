#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <glm/vec3.hpp>
#include<GL/gl.h>


class TriangleWindow;
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class Particle {
public :
    Particle();
    ~Particle();

    void setName(std::string);
    std::string getName() const;

    void setSize(unsigned char);
    unsigned char getSize() const;

    void setColor(unsigned char, unsigned char, unsigned char);
    struct Color getColor() const;

    void setPosition(GLfloat, GLfloat, GLfloat);
    glm::vec3 getPosition() const;


     void setLifeTime(unsigned int);
     unsigned int getLifeTime() const;

    bool live(TriangleWindow*);
    void move();
    void clear();
    void copy(Particle*);
private :
    std::string name;
    unsigned char size;
    struct Color color;
    glm::vec3 position;
    unsigned int lifeTime;
};

#endif // PARTICLE_H
