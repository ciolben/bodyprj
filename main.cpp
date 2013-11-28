#include "mainwindow.h"
#include <QApplication>

#include "viewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    // Instantiate the viewer.
    Viewer viewer;
    viewer.setWindowTitle("Bodyprj");

    // Make the viewer window visible on screen.
    viewer.show();

    return a.exec();
}

