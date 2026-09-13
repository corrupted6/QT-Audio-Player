//Custom window that houses GUI
class Window : public QMainWindow {
	
	public:
		Window(Queue *q) {
			QMainWindow();
			queue = q;
			init();
		}

		~Window() {
			std::cout << "window gone";

			delete topBar;
			delete uploadMenu;
			delete bottomBar;
			delete frame;
			delete progBar;
			delete skipBack;
			delete skipForward;
			delete pause;
		}

		void scrubBack() {
			qint64 pos = player->position() - 5000;
			pos -= pos % 1000;
			player->setPosition(pos);
			std::cout << "back";
		}

		void scrubForward() {
			qint64 pos = player->position() + 5000;
			pos -= pos % 1000;
			player->setPosition(pos);
		}

	private:
		//sooo many pointers because qt is built that way
		Queue *queue;
		QTimer *timer;

		QFrame *mainFrame;
		QMenuBar *topBar;
		QMenu *uploadMenu;
		QToolBar *bottomBar;
		QLabel *songName;
		QLabel *time;
		QProgressBar *progBar;
		QPushButton *skipBack;
		QPushButton *skipForward;
		QPushButton *pause;
		QFrame *frame;
		QVBoxLayout *scrollLay;

		vector<QPushButton*> songsInQueue;

		void keyPressEvent(QKeyEvent *event) override {
			std::cout << "key";
        	if (event->key() == Qt::Key_Right || event->key() == Qt::Key_L) {
            	scrubForward();
        	} else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_J) {
            	scrubBack();
            } else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_K) {
            	pauseplay();
        	} else {
            	QMainWindow::keyPressEvent(event); // let base handle others ig
        	}
    	}

		void updateTime() {
			time->setText(queue->getFormattedTime());
			progBar->setValue(queue->getCurrentProgress());
			if (player->isPlaying() && queue->isDonePlaying()) {
				next();
			}
		}

		void updateName() {
			songName->setText(queue->getSongName());
		}
		
		void pauseplay() {
			if (player->isPlaying()) {
				player->pause();
			} else {
				player->play();
			}
		}

		void next() {
			player->stop();
			queue->next();
			player->setSource(queue->getCurrentPath());
			updateName();
			player->play();
		}

		void prev() {
			player->stop();

			//so u can go to beginning of song or go to last
			if (queue->getCurrentSeconds() <= 5) {
				queue->prev();
				player->setSource(queue->getCurrentPath());
				updateName();
			}

			player->play();
		}

		void populateScrollArea(QVBoxLayout* lay) {

			for (int i = songsInQueue.size() - 1; i >= 0; i--) {
				delete songsInQueue[i];
			}
			songsInQueue.clear();

			vector<string> names = queue->getSongNames();
			vector<string> artists = queue->getSongArtists();
			vector<string> paths = queue->getSongPaths();
			for (int i = 0; i < names.size(); i++) {
				//artist, name, path
				SongButton *butt = new SongButton(artists[i], names[i], paths[i]);
				lay->addWidget(butt);
				songsInQueue.push_back(butt);
			}
		}

		void init() {
			this->setFixedSize(720, 480);
			this->setWindowTitle("SUSSY DIDDY Audio Player boiiii");
			
			QObject::connect(player, QMediaPlayer::playbackStateChanged, this, updateTime);

			topBar = this->menuBar();
			uploadMenu = topBar->addMenu("&Library");
			
			//make timer
			timer = new QTimer(this);
			QObject::connect(timer, QTimer::timeout, this, Window::updateTime);
    		timer->start(200); // update every 200ms

			//make toolbar	
			bottomBar = new QToolBar(this);
			songName = new QLabel("Name");
			time = new QLabel("0:00/0:00");
			skipBack = new QPushButton("<<");
			skipForward = new QPushButton(">>");
			pause = new QPushButton("|| / >");
			progBar = new QProgressBar();
			mainFrame = new QFrame();

			//config progressbar
			QString pbfrmt = "";
			progBar->setRange(0,100);
			progBar->setFormat(pbfrmt);
			progBar->setStyleSheet("QProgressBar { margin: 0.5px;border: 2px solid grey; } QProgressBar::chunk { background-color: #AF1F1F; }");

			//set things to have no focus so arrow keys work
			skipBack->setFocusPolicy(Qt::NoFocus);
			skipForward->setFocusPolicy(Qt::NoFocus);
			pause->setFocusPolicy(Qt::NoFocus);
			
			//so media buttons work
			QObject::connect(pause, QPushButton::clicked, this, pauseplay);
			QObject::connect(skipForward, QPushButton::clicked, this, next);
			QObject::connect(skipBack, QPushButton::clicked, this, prev);

			//add it all
			bottomBar->addWidget(songName);
			bottomBar->addWidget(skipBack);
			bottomBar->addWidget(pause);
			bottomBar->addWidget(skipForward);
			bottomBar->addWidget(progBar);
			bottomBar->addWidget(time);

			// make scroll stuff
			frame = new QFrame();
			scrollLay = new QVBoxLayout(frame);
			populateScrollArea(scrollLay);

			// put in scroll area
			QScrollArea *scrollArea = new QScrollArea();
			scrollArea->setWidget(frame);
			scrollArea->setFocusPolicy(Qt::NoFocus);
			scrollArea->setWidgetResizable(true);

			//add everything
			this->addToolBar(Qt::BottomToolBarArea, bottomBar);
			this->setCentralWidget(scrollArea);
			
			//start playing for now
			player->setAudioOutput(audio);
			player->setSource(queue->getCurrentPath());
			QUrl ppp = queue->getCurrentPath();
			QString pppp = ppp.toLocalFile();
			std::cout << pppp.toUtf8().constData() << "printeddddd";
			player->play();
			updateName();

		}

};