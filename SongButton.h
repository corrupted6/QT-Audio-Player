//custom button that stores song info and does some setup for me :3
class SongButton : public QPushButton {
public:
	SongButton(string ar, string na, string pa) {
		QPushButton();
		//info
		artist = ar;
		name = na;
		path = pa;
		//button setup
		this->setFocusPolicy(Qt::NoFocus);
	}
private:
	string artist, name, path;


};