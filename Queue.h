//like a queue of songs :3
class Queue {
	
	public:
		Queue() {
			parseAudioFolder();
		}

		const QString getSongName() {
			return QString::fromStdString(fileInfo[index][1]);
		}

		const QUrl getCurrentPath() {
			return QUrl::fromLocalFile(QString::fromStdString(fileInfo[index][2]));
		}

		QString getFormattedTime() {
			qint64 len = player->duration()/1000;
			qint64 pos = player->position()/1000;
			int minl = 0, secl = 0, minp = 0, secp = 0;

			while (len >= 60) {
				minl++;
				len -= 60;
			}
			secl = (int)len;

			while (pos >= 60) {
				minp++;
				pos -= 60;
			}
			secp = (int)pos;

			//format the string ugh
			QString out = "";
			out += std::to_string(minp) + ":";
			if (secp < 10) {out += "0";}
			out += std::to_string(secp) + " / " + std::to_string(minl) + ":";
			if (secl < 10) {out += "0";}
			out += std::to_string(secl);

			return out;
		}

		int getCurrentSeconds() {
			qint64 pos = player->position()/1000;
			return (int)pos;
		}
		//int 0-100 for percent
		int getCurrentProgress() {
			//int division rounds down ;3
			double pos = player->position()/1000.0;
			double dur = player->duration()/1000.0;
			return (int)((pos/dur)*100);
		}

		void next() {
			if (index < fileInfo.size()-1) {
				index += 1;
			} else {
				std::cout << "tried to go forward at end of list hahahahahahaa";
			}
		}

		void prev() {
			if (index > 0) {
				index -= 1;
			} else {
				std::cout << "tried to go back at beginning of list hahahahahahaa";
			}
		}

		bool isDonePlaying() {
			return player->position() >= player->duration();
		}

		vector<string> getSongNames() {
			vector<string> out(fileInfo.size());
			for (int i = 0; i < fileInfo.size(); i++) {
				out[i] = fileInfo[i][1];
			}
			return out;
		}

		vector<string> getSongArtists() {
			vector<string> out(fileInfo.size());
			for (int i = 0; i < fileInfo.size(); i++) {
				out[i] = fileInfo[i][0];
			}
			return out;
		}

		vector<string> getSongPaths() {
			vector<string> out(fileInfo.size());
			for (int i = 0; i < fileInfo.size(); i++) {
				out[i] = fileInfo[i][2];
			}
			return out;
		}

	private:

		vector<vector<string>> fileInfo;
		int index = 0;
		
		void parseAudioFolder() {
			fileInfo.clear();

			//get path of audio folder
			path thisPath = __FILE__;
			path audioFolder = thisPath.parent_path() / "audio";
			string currPath;
			std::cout << "Files in " << audioFolder << "\n";
	
			//parse through the files in that path and read them :/
			for (const auto& file : directory_iterator(audioFolder)) {
				currPath = file.path().string();
				std::cout << currPath << "\n";
				string filename = currPath.substr(currPath.find_last_of("\\")+1);
				vector<string> song;
				//if dash, then it's artist-song
				//format is artist-name-path
				if (filename.find("-") != string::npos) {
					song.push_back(filename.substr(0, filename.find("-"))); //artist
					song.push_back(filename.substr(filename.find("-")+1)); //songname
					song.push_back(currPath);//path
				} else {
					//otherwise, filepath as placeholder
					song.push_back(filename);
					song.push_back(filename);
					song.push_back(currPath);
				}

				fileInfo.push_back(song);
			}
			
			index = 0;

			std::cout << "fileInfo: " << "\n";
			for (vector<string> s : fileInfo) {
				for (string i : s) {
					std::cout << i << "\n";
				}
			}
		}
};