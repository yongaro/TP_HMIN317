#ifndef CAMERA_H
#define CAMERA_H


#include <QtGui/QOpenGLShaderProgram>
#include <glm/vec3.hpp>
#include <QKeyEvent>

class Camera {
    protected :
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 forward;
        glm::vec3 left;

        GLfloat speed;
        GLfloat sensivity;

        GLfloat theta;
        GLfloat phi;

        bool booster;
        float boost;

        void vectorsFromAngles();
        void setTheta(GLfloat);
        void setPhi(GLfloat);
     public :
        Camera();
        Camera(GLfloat, GLfloat, GLfloat);
        ~Camera();

        void see(GLfloat, GLfloat, GLfloat);

        // Gestion de la caméra
        void toForward();
        void toBackward();
        void toLeft();
        void toRight();
        void toUp();
        void toDown();

        GLfloat getRealSpeed() const;

        GLfloat getX() const;
        GLfloat getY() const;
        GLfloat getZ() const;

        void mouseMove(int, int);

        // Settings
        void setBooster(bool);
        void setSpeed(GLfloat);
        void setSensivity(GLfloat);
        void setBoost(GLfloat);

        void context(QMatrix4x4 &);
        void clear();
};

#endif // CAMERA_H
