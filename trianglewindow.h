#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "camera.h"
#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QTimer>
#include <QObject>
#include "season.h"
#include "pool.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>


class TriangleWindow : public OpenGLWindow {
Q_OBJECT
public:
    TriangleWindow(const unsigned char fps = 60);
    ~TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void keyReleaseEvent(QKeyEvent*);
    void keyPressEvent(QKeyEvent*);
    void mouseMoveEvent(QMouseEvent *event);
    void run();

    void setSeason(const unsigned char);
    bool isOverTextureHeight(GLfloat, GLfloat, GLfloat);
    void increaseTextureHeight(GLfloat, GLfloat, GLfloat);
    void decreaseTextureHeight(GLfloat, GLfloat, GLfloat);
    void clear();
    void updateParticles();

    static const unsigned char WINTER = 0;
    static const unsigned char SPRING = 1;
    static const unsigned char SUMMER = 2;
    static const unsigned char AUTOMN = 3;

public slots:
    void nextSeason();

private:
    void initCamera();
    void initTimer();
    void initMap();
    void initSeason();

    Pool<Particle> *particles;
    std::vector<GLfloat> particlesPos;
    std::vector<GLfloat> particlesCol;
    std::vector<GLfloat> particlesUV;
    std::vector<GLfloat> particlesNrm;
    //Une ou deux lignes bien visibles
    std::vector<GLfloat> testP_pos;
    std::vector<GLfloat> testP_col;
    std::vector<GLfloat> testP_nrm;

    bool _hold; //au départ on part du principe que le bouton n'est pas maintenu
    bool on;
    QTimer *timer;
    Season* seasons;
    char currentSeason;
    GLuint loadShader(GLenum type, const char *source);
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_uvAttr;
    GLuint m_nrmAttr;
    GLuint m_matrixUniform;
    GLuint m_camPosUniform;
    GLfloat m_step;
    unsigned int tick;
    unsigned char m_fps;

    unsigned int m_img_w;
    unsigned int m_img_h;

    Camera m_cam;
    QImage m_img;
    std::vector<GLfloat> triangles;
    std::vector<GLfloat> uv;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> nrm;
    GLfloat **groundHeight;
    QOpenGLShaderProgram* m_program;
    int m_frame;

    void forwardCam();
    void rearwardCam();
    void towardLeftCam();
    void towardRightCam();
    void upCam();
    void downCam();

    // Utilitaires
    void addPointToTriangles (const unsigned int, const unsigned int, const unsigned int);
    glm::vec3 getNormal(const unsigned int);
    float YfromHeightMap(GLfloat, GLfloat);
    void setFps(const unsigned char);

};

#endif // TRIANGLEWINDOW_H
