class Board {
public:
	Board();
	~Board();
	void getState();
	void Move(String from, String to);
private: 
	checkForCheck();
	checkForCheckMate();
	checkLegalMove();
};