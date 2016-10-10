#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT
    public:
        explicit OpenGLWindow(QWindow *parent = 0);
        ~OpenGLWindow();

        virtual void render(QPainter *painter);
        virtual void render();

        virtual void initialize();

        void setAnimating(bool animating);

    public slots:
        void renderLater();
        void renderNow();
        void nextSeason();

    protected:
        bool event(QEvent *event) Q_DECL_OVERRIDE;

        void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

    private:
        bool m_update_pending;
        bool m_animating;

        QOpenGLContext *m_context;
        QOpenGLPaintDevice *m_device;
};

#endif // OPENGLWINDOW_H
