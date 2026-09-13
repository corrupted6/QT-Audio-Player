//INCLUDES

#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <thread>

using std::vector;
using std::string;
using std::ifstream;
using namespace std::filesystem;

#include <QString>
#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QWidget>
#include <QProgressBar>
#include <QScrollArea>
#include <QTimer>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QFrame>

//GLOBAL VARIABLES

	//QT
	QMediaPlayer *player = new QMediaPlayer();
	QAudioOutput *audio = new QAudioOutput();

//INCLUDES THAT NEED THE VARIABLES
#include "Queue.h"
#include "SongButton.h"
#include "Window.h"

//METHODS

int main(int argc, char**argv) {
	QApplication app(argc, argv);
	Queue *q = new Queue();
	Window win(q);
	win.show();


	return app.exec();
}
