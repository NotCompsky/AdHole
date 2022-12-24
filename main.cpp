#include <QApplication>
#include "adhole.hpp"

int main(int argc,  char *argv[]){
    QApplication app(argc, argv);

    ListManager window;
    window.show();

    return app.exec();
}
