#include <QApplication>
#include "adhole.hpp"

int main(int argc,  char *argv[]){
	if (argc != 2){
		write(2, "Usage: AdHole [/path/to/adhole/settings]\n", 41);
		return 1;
	}

    QApplication app(argc, argv);

	ListManager window(nullptr, argv[1]);
    window.show();

    return app.exec();
}
