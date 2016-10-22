#include "trianglewindow.h"
#include <QtGui/QGuiApplication>

#include "pool.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;

    format.setSamples(16);


    //Pool<TriangleWindow> poolTW (1);
    //TriangleWindow w120  (120);
    TriangleWindow w60(60);
    //TriangleWindow w30(30);
    //TriangleWindow w1(1);

    w60.setTitle("OpenGL Qt Drama");
    //w30.setTitle("OpenGL Qt Drama");

    //w1.setTitle("1");

    //w120.run();
    w60.run();
    //w30.run();
    w60.resize(640,480);
    //w1.run();

    //w120.setSeason(TriangleWindow::WINTER);
    w60.setSeason(TriangleWindow::AUTOMN);
    //w30.setSeason(TriangleWindow::WINTER);
    //w1.setSeason(TriangleWindow::AUTOMN);


    QTimer calendar;
    //QObject::connect(&calendar, SIGNAL(timeout()), &w120,  SLOT(nextSeason()));
    QObject::connect(&calendar, SIGNAL(timeout()), &w60,  SLOT(nextSeason()));
    //QObject::connect(&calendar, SIGNAL(timeout()), &w30,  SLOT(nextSeason()));
    //QObject::connect(&calendar, SIGNAL(timeout()), &w1,  SLOT(nextSeason()));
    calendar.start(15000);

    return app.exec();
}
