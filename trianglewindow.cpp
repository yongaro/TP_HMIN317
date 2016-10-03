#include "trianglewindow.h"
#include "camera.h"

#include <QKeyEvent>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>

#include <QTimer>
#include <iostream>
#include "season.h"

using namespace std;

static const char* vertexShaderSource =
    "attribute vec3 posAttr;\n"
    "attribute vec3 colAttr;\n"
    "attribute vec2 uvAttr;\n"
    "attribute vec3 nrmAttr;\n"
    "uniform sampler2D sampler;\n"
    "varying vec4 col;\n"
    "varying vec3 fragNrm;\n"
    "uniform mat4 matrix;\n"
    "uniform vec3 camPos;\n"

    "void main() {\n"
    "   vec4 surfaceColor = texture2D(sampler, uvAttr);\n"
    "   if( surfaceColor.x < 0.7 ){ surfaceColor *= vec4(0.5, 1.0, 0.5, 1.0); }\n"
    "   fragNrm = nrmAttr;\n"
    "   col = surfaceColor;\n"
    "   gl_Position = matrix * vec4(posAttr, 1.0);\n"
    "}\n";

static const char* fragmentShaderSource =
    "varying highp vec4 col;\n"
    "varying highp vec3 fragNrm;\n"
    "uniform mat4 matrix;\n"
    "uniform vec3 camPos;\n"

    "vec3 L = normalize( vec3(1.0, 1.0, 0.0) );\n"
    "vec3 V = normalize( camPos );\n"
    "vec3 N = normalize( fragNrm );\n"
    "vec3 R = reflect(-L, N);\n"
    "vec3 ambient = vec3(0.01, 0.01, 0.01);\n"
    "vec3 diffuse = max(dot(N, L), 0.0) * col.rgb;\n"
    "float shininess = 16.0;\n"
    "vec3 specular = pow(max(dot(R, V), 0.0), shininess) * vec3(0.2, 0.2, 0.2);\n"

    "void main() {\n"
    "   if( (gl_Color.x > 0.0 && gl_Color.y > 0.0) && gl_Color.z > 0.0 ){ gl_FragColor = gl_Color; }\n"
    "   else{ gl_FragColor = vec4(ambient + diffuse + specular, 1.0); }\n"
    "}\n";



TriangleWindow::~TriangleWindow() {
    delete timer;
    delete[] seasons;
}

TriangleWindow::TriangleWindow(const unsigned char fps)
    : on(false),
      m_step(0.05f),
      tick(0),
      m_fps(fps),
      m_program(0),
      m_frame(0)
{
    particles = Pool<Particle> (200);
    initCamera();
    initTimer();
    initMap();
    initSeason();
}

void TriangleWindow::initCamera() {
    m_cam = Camera(0.0f, 3.0f, 5.0f);
    m_cam.see(1.0f, 2.0f, 1.0f);
    m_cam.setSpeed(m_step);
    m_cam.setBoost(10.0f);
    m_cam.setSensivity(0.3f);
}

void TriangleWindow::initTimer() {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    timer->start(1000 / m_fps);
}

void TriangleWindow::initMap() {
    if(m_img.load("./heightmap-1.png")) {
        m_img_w = m_img.width();
        m_img_h = m_img.height();
    } else {
        cerr << "impossible de charger la texture" << endl;
    }
}

void TriangleWindow::initSeason() {
    seasons = new Season [4];
    seasons[0] = Season::winter();
    seasons[1] = Season::spring();
    seasons[2] = Season::summer();
    seasons[3] = Season::automn();

    currentSeason = 0;
}

void TriangleWindow::setFps (const unsigned char newFps) {
    if (newFps > 0) {
        m_fps = newFps;
        timer->setInterval(1000 / newFps);
    }
}

float TriangleWindow::YfromHeightMap(GLfloat x, GLfloat z){
   // On transforme les coordonée 3D en coordonnées images
    unsigned int xp = (x / m_step);
    unsigned int yp = (z / m_step);

    // Lecture du pixel
    if((xp < m_img_w) && (yp < m_img_h)){
        QColor pix = m_img.pixel(xp, yp);
        return (GLfloat)pix.red() / 255.0f * 2.0f;
    }
    return 0.0f;
}

void TriangleWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        m_cam.mouseMove(event->x(), event->y());
    }
}

void TriangleWindow::keyReleaseEvent(QKeyEvent* key) {
    switch (key->key()) {
        case Qt::Key_Shift :
            m_cam.setBooster(false);
            break;
    }
}

void TriangleWindow::keyPressEvent(QKeyEvent* key) {
    // On teste la touche

    switch (key->key()) {

    case Qt::Key_Z :
            this->forwardCam();
            break;

        case Qt::Key_S :
            this->rearwardCam();
            break;

        case Qt::Key_D :
            this->towardLeftCam();
            break;

        case Qt::Key_Q :
            this->towardRightCam();
            break;

        case Qt::Key_Up :
            this->upCam();
            break;

        case Qt::Key_Down :
            this->downCam();
            break;

        case Qt::Key_C :
            this->on = !on;
            break;

        case Qt::Key_P :
            setFps(m_fps * 2);
            break;

        case Qt::Key_M :
            setFps(m_fps / 2);
            break;

        case Qt::Key_Shift :
            m_cam.setBooster(true);
            break;

        default:
            break;
    }
}

GLuint TriangleWindow::loadShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::addPointToTriangles (const unsigned int x, const unsigned int y, const unsigned int z) {
    float px = x * m_step;
    float pz = z * m_step;
    float py = YfromHeightMap(px,pz);

    triangles.push_back( px );
    triangles.push_back( py );
    triangles.push_back( pz );

    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);

    uv.push_back( (GLfloat)x/(m_img_w -1));
    uv.push_back( (GLfloat)z/(m_img_h -1));
}

glm::vec3 TriangleWindow::getNormal(const unsigned int fpIndex){
    glm::vec3 p1 = glm::vec3(triangles.at(fpIndex),triangles.at(fpIndex+1),triangles.at(fpIndex+2));
    glm::vec3 p2 = glm::vec3(triangles.at(fpIndex+3),triangles.at(fpIndex+4),triangles.at(fpIndex+5));
    glm::vec3 p3 = glm::vec3(triangles.at(fpIndex+6),triangles.at(fpIndex+7),triangles.at(fpIndex+8));

    glm::vec3 v1 = p2-p1;
    glm::vec3 v2 = p3-p1;

    return glm::cross(v1,v2);
}


void TriangleWindow::initialize() {
    _hold = false;
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_uvAttr = m_program->attributeLocation("uvAttr");
    m_nrmAttr = m_program->attributeLocation("nrmAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    m_camPosUniform = m_program->uniformLocation("camPos");

    // Creation des triangles
    for (GLuint x = 0; x < m_img_w; ++x) {
        for (GLuint y = 0; y < m_img_h; ++y) {
            // Triangle haut - droit
            this->addPointToTriangles(x, 0, y);
            this->addPointToTriangles(x + 1, 0, y + 1);
            this->addPointToTriangles(x + 1, 0, y);
            glm::vec3 n1 = getNormal(triangles.size()-9);
            for( size_t i = 0; i < 3; ++i ){
                nrm.push_back(n1.x); nrm.push_back(n1.y); nrm.push_back(n1.z);
            }

            // Triangle bas - gauche
            this->addPointToTriangles(x, 0, y);
            this->addPointToTriangles(x, 0, y + 1);
            this->addPointToTriangles(x + 1, 0, y + 1);
            glm::vec3 n2 = getNormal(triangles.size()-9);
            for( size_t i = 0; i < 3; ++i ){
                nrm.push_back(n2.x); nrm.push_back(n2.y); nrm.push_back(n2.z);
            }
        }
    }
    //copie des information crees dans les vertex array
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, triangles.data() );
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data() );
    glVertexAttribPointer(m_uvAttr, 2, GL_FLOAT, GL_FALSE, 0, uv.data() );
    glVertexAttribPointer(m_nrmAttr, 3, GL_FLOAT, GL_FALSE, 0, nrm.data() );

    //Chargement de la texture dans une QImage, passage des donnees bruts a openGL et definition de l interpolation

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_img_w, m_img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_img.bits() );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glDepthRange(0,1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
}


bool TriangleWindow::isOverTextureHeight(GLfloat x, GLfloat y, GLfloat z) {
   // On transforme les coordonée 3D en coordonnées images
    unsigned int xp = (x / m_step);
    unsigned int yp = (z / m_step);

    // Lecture du pixel
    if((xp < m_img_w) && (yp < m_img_h)){
        QColor pix = m_img.pixel(xp, yp);
        return ((y) > ((GLfloat (pix.red()) / 255.0f) * 2.0f));
    } else {
        return true;
    }
}

void TriangleWindow::forwardCam() {
    if (this->isOverTextureHeight(m_cam.getX(), m_cam.getY(), m_cam.getZ() + m_step)) {
        m_cam.toForward();
    }
}

void TriangleWindow::rearwardCam() {
    if (this->isOverTextureHeight(m_cam.getX(), m_cam.getY(), m_cam.getZ() - m_step)) {
        m_cam.toBackward();
    }
}

void TriangleWindow::towardRightCam() {
    if (this->isOverTextureHeight(m_cam.getX() + m_step, m_cam.getY(), m_cam.getZ())) {
        m_cam.toRight();
    }
}

void TriangleWindow::towardLeftCam() {
    if (this->isOverTextureHeight(m_cam.getX() - m_step, m_cam.getY(), m_cam.getZ())) {
        m_cam.toLeft();
    }
}

void TriangleWindow::upCam() {
    if (this->isOverTextureHeight(m_cam.getX(), m_cam.getY() + m_step, m_cam.getZ())) {
        m_cam.toUp();
    }
}

void TriangleWindow::downCam() {
    if (this->isOverTextureHeight(m_cam.getX(), m_cam.getY() - m_step, m_cam.getZ())) {
        m_cam.toDown();
    }
}

void TriangleWindow::render() {
    ++tick;
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    //glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();

    QMatrix4x4 matrix;

    matrix.perspective(90.0f, 4.0f/3.0f, 0.001f, 100.0f);
    m_cam.context(matrix);

    if (on) {
        matrix.rotate(tick, 0, 1.f, 0.0f);
    }

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_camPosUniform, QVector3D(m_cam.getX(), m_cam.getY()+1.0f, m_cam.getZ()));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, triangles.data() );
    glDrawArrays(GL_TRIANGLES, 0,  triangles.size() / 3);




    // Ajout de nouvelles particules

    Particle* p;
    for (int i=0; i < 1; ++i) {
         p = particles.ask();

         //p->setName("Hello");
         p->copy(seasons[currentSeason].getParticle());
         p->setPosition(0.1f, 1.0f, 0.1f);
    }

    // Vie des particules
    std::list<Particle*>::iterator it = particles.used.begin();
   /*
    while (i != items.end())
    {
        bool isActive = (*i)->update();
        if (!isActive)
        {
            items.erase(i++);  // alternatively, i = items.erase(i);
        }
        else
        {
            other_code_involving(*i);
            ++i;
        }
    }
    */

    //glPointSize(1);
    //glColor3ub(255,0,0);
    //glBegin(GL_POINT);

    particlesPos.clear();
    while (it != particles.used.end()) {
        bool isActive = (*it)->live(this);
        //cout << isActive << endl;
        if (isActive) {
            glm::vec3 pos = (*it)->getPosition();
            //cout << "t:" << (*it)->getLifeTime() << " ("<< pos.x << ";" << pos.y << ";" << pos.z << ")" << endl;
            GLint r = (*it)->getColor().r;
            GLint g = (*it)->getColor().g;
            GLint b = (*it)->getColor().b;

            particlesPos.push_back(pos.x);
            particlesPos.push_back(pos.y);
            particlesPos.push_back(pos.z);
            //glVertex3f(pos.x, pos.y, pos.z);
        }

        if (!isActive) {
            particles.recycle(*it);
            //articles.avaibles.push_back(*it);
           particles.used.erase(it++);

        } else {
            ++it;
        }

    }

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, particlesPos.data() );
    glDrawArrays(GL_POINTS, 0,  particlesPos.size() / 3);

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    //glEnd();
    //glFlush();
    m_program->release();

    ++m_frame;
}

void TriangleWindow::clear() {}

void TriangleWindow::run() {
    QSurfaceFormat format;
    setFormat(format);
    resize(400, 300);
    show();
}

void TriangleWindow::setSeason(const unsigned char season) {
    if (season < 4) {
        currentSeason = season;
    }
}

void TriangleWindow::nextSeason() {
    while(particles.used.size() > 0) {
     Particle* p = particles.used.front();
     particles.used.pop_front();
     particles.recycle(p);
     particles.avaibles.push_back(p);
    }

    currentSeason = (currentSeason + 1) % 4;
    cout << title().toStdString() << " : " << seasons[currentSeason].getName() << endl;
}
