#include "camera.h"
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <QKeyEvent>

using namespace std;

Camera::Camera() :
    speed(.05f),
    theta(.0f),
    phi(.0f)
{}

Camera::Camera(GLfloat x, GLfloat y, GLfloat z) {
    position = glm::vec3(x, y, z);
    theta = 0.0f;
    phi = 0.0f;
    booster = false;
}


Camera::~Camera() {}

void Camera::clear() {}

void Camera::vectorsFromAngles() {
    static const glm::vec3 up(0,1,0);

    // On limite phi : pas de loopings
    if (phi > 89)
        phi = 89;
    else if (phi < -89)
        phi = -89;

    // coordonnées sphériques -> cartésiennes
    double r_temp = cos(phi  * M_PI/180);
    forward.y = -0.5f + sin(phi * M_PI/180) / 2;
    forward.x = r_temp * cos(theta * M_PI/180);
    forward.z = r_temp * sin(theta * M_PI/180);
    left = glm::cross(up, forward);
    glm::normalize(left);

    // on calcule ce que regarde la caméra (la cible)
    target = position + forward;
}

void Camera::toForward() {
    double v = getRealSpeed();
    position.x += forward.x * v;
    position.z += forward.z * v;
}

void Camera::toBackward() {
    double v = getRealSpeed();;
    position.x -= forward.x * v;
    position.z -= forward.z * v;
}

void Camera::toLeft() {
    double v = getRealSpeed();
    position.x -= left.x * v;
    position.z -= left.z * v;
}

GLfloat Camera::getRealSpeed() const {
    return(booster ? boost * speed : speed);
}
void Camera::toRight() {
    double v = getRealSpeed();
    position.x += left.x * v;
    position.z += left.z * v;
}

void Camera::toUp() {
    position.y += getRealSpeed();
}

void Camera::toDown() {
    position.y -= getRealSpeed();
}

// FIXIT : transformer en coordonée sphérique et stocker dans phi et theta
void Camera::see(GLfloat x, GLfloat y, GLfloat z) {
    forward = glm::vec3(x, y, z) - position;
}

GLfloat Camera::getX() const {
    return position.x;
}

GLfloat Camera::getY() const {
    return position.y;
}

GLfloat Camera::getZ() const {
    return position.z;
}

void Camera::setBooster(bool enable) {
    booster = enable;
}

void Camera::setTheta(GLfloat theta) {
    this->theta = theta;
}

void Camera::setPhi(GLfloat phi) {
    this->phi = phi;
}

void Camera::setSpeed(GLfloat speed) {
    this->speed = speed;
}

void Camera::setBoost(GLfloat boost) {
    this->boost = boost;
}

void Camera::setSensivity(GLfloat sensivity) {
    this->sensivity = sensivity;
}

void Camera::mouseMove(int x, int y) {
   setTheta(x * sensivity);
   setPhi(y * sensivity);
}

void Camera::context(QMatrix4x4& context) {
    // Recalcul des coordonées position - vue
    vectorsFromAngles();
    target = position + forward;
    context.lookAt(
        QVector3D(position.x, position.y, position.z),
        QVector3D(target.x, target.y, target.z),
        QVector3D(0.0f, 1.0f, 0.0f)
    );
}
