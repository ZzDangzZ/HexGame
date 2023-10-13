#include<iostream>
#include<vector>

using namespace std;

enum class Player : short { Blue, Red };

ostream& operator<<(ostream& out, const Player& p) {
	switch (p) {
	case Player::Blue: out << "Blue"; break;
	case Player::Red: out << "Red"; break;
	}
	return out;
}

enum class SquareVal : short { Blank = 0, Blue = 1, Red = 2 };

ostream& operator<<(ostream& out, const SquareVal& s) {
	switch (s) {
	case SquareVal::Blank: out << "0 "; break;
	case SquareVal::Blue: out << "1 "; break;
	case SquareVal::Red: out << "2 "; break;
	}
	return out;
}


class Board {
public:
	//initialized an empty board
	Board(int n) : board(n, vector<SquareVal>(n, SquareVal::Blank)) {
		size = n;
		totalNodes = size * size;
	}
	~Board();
	SquareVal getSquare(int i, int j);
	void setSquare(int i, int j, SquareVal v);
	int getNode(int i, int j);
	bool isWithinBoard(int i, int j);
	bool adjacent(int i, int j, int x, int y);
	bool winnerCheck(Player p);
	bool DFS(vector<vector<int>> marker, int i, int j, Player p);
	bool isEndNode(int i, int j, Player p);
	void printBoard();

private:
	int size;
	int totalNodes;
	vector<vector<SquareVal>> board;
};

Board::~Board()
{
	board.clear();
}

//get the value of square[i][j]
SquareVal Board::getSquare(int i, int j)
{
	return board[i][j];
}

//setting the value of square[i][j]
void Board::setSquare(int i, int j, SquareVal v)
{
	board[i][j] = v;
}

//get the node of square[i][j]
int Board::getNode(int i, int j)
{
	int n;
	n = i * size + j;
	return n;
}

//checking if the move is within board
bool Board::isWithinBoard(int i, int j)
{
	return i >= 0 && i < board.size() && j >= 0 && j < board.size();
}

//checking if square[x][y] is within range of suqare[i][j]
bool Board::adjacent(int i, int j, int x, int y)
{
	return((i == x && j == y - 1)
		|| (i == x && j == y + 1)
		|| (i == x - 1 && j == y)
		|| (i == x + 1 && j == y)
		|| (i == x - 1 && j == y + 1)
		|| (i == x + 1 && j == y - 1));
}

//checking if there is a winner
bool Board::winnerCheck(Player p)
{
	int r, c;
	vector<vector<int>> marker;
	//initialize an empty marker
	for (int x = 0; x < size; x++) {
		vector<int> e;
		for (int y = 0; y < size; y++) {
			e.push_back(0);
		}
		marker.push_back(e);
	}

	//runninng Depth First Search based on current player
	for (int i = 0; i < size; i++) {
		if (p == Player::Blue) {
			c = i;
			r = 0;
		}
		else {
			c = 0;
			r = i;
		}

		if (marker[0][i] == 0) {
			bool result = DFS(marker, r, c, p);
			if (result)
				return true;
		}
	}
	return false;
}

//Depth First Search
bool Board::DFS(vector<vector<int>> marker, int i, int j, Player p)
{
	//setting current value based on the current player
	auto currentVal = SquareVal::Blue;
	if (p == Player::Red) {
		currentVal = SquareVal::Red;
	}

	if (i < 0 || i >= size || j < 0 || j >= size) {
		return false;
	}

	if (marker[i][j] == 1) {
		return false;
	}

	if (board[i][j] != currentVal) {
		return false;
	}

	//if it is the endNode then stop
	marker[i][j] = 1;
	if (isEndNode(i, j, p)) {
		return true;
	}
	//recursive code to continue checking the adjacent node
	if (this->DFS(marker, i - 1, j, p)) {
		return true;
	}
	if (this->DFS(marker, i + 1, j, p)) {
		return true;
	}
	if (this->DFS(marker, i, j - 1, p)) {
		return true;
	}
	if (this->DFS(marker, i, j + 1, p)) {
		return true;
	}
	if (this->DFS(marker, i - 1, j + 1, p)) {
		return true;
	}
	if (this->DFS(marker, i + 1, j - 1, p)) {
		return true;
	}

	return false;
}

//checking if it is the end node based on the current player
bool Board::isEndNode(int i, int j, Player p)
{
	if (p == Player::Blue && i >= size - 1) {
		return true;
	}
	if (p == Player::Red && j >= size - 1) {
		return true;
	}
	return false;
}

//print the board
void Board::printBoard()
{
	for (int i = 0; i < board.size(); i++) {
		for (int count = -1; count < i; count++)
			cout << " ";
		for (int j = 0; j < board[i].size(); j++) {
			if (board[i][j] == SquareVal::Blank) {
				if (j != board[i].size() - 1) {
					cout << " . " << "-";
				}
				else
					cout << " . ";
			}
			else if (board[i][j] == SquareVal::Blue) {
				if (j != board[i].size() - 1) {
					cout << " X " << "-";
				}
				else
					cout << " X ";
			}
			else {
				if (j != board[i].size() - 1) {
					cout << " O " << "-";
				}
				else
					cout << " O ";
			}
		}
		cout << endl;
		for (int count = -1; count < i; count++)
			cout << " ";
		cout << "  \\";
		for (int i = 1; i < size; i++) {
			cout << " / \\";
		}
		cout << endl;
	}
}


class HexGame {
public:
	//Game always start with Blue going first, so default first player is Blue
	HexGame(int n) : board(n), player(Player::Blue), isWon(false) { size = n; }
	bool moveIsValid(int i, int j);
	void move(int i, int j);
	void printBoard();
	Player getPlayer();
	bool gameEnd();

private:
	Player player;
	Board board;
	bool isWon;
	int size;
};

//checking if move is legal
bool HexGame::moveIsValid(int i, int j)
{
	return board.isWithinBoard(i, j) && board.getSquare(i, j) == SquareVal::Blank;
}

//player's move
void HexGame::move(int i, int j)
{
	if (moveIsValid(i, j)) {
		SquareVal val = player == Player::Blue ? SquareVal::Blue : SquareVal::Red;
		board.setSquare(i, j, val);
	}

	//print game board 
	cout << endl;
	board.printBoard();
	cout << endl;

	//print out message when the current player win
	if (gameEnd() == true) {
		cout << "Congratulations! Player " << player << " has won";
		cout << endl;
	}
	else
		//switch player
		player = player == Player::Blue ? Player::Red : Player::Blue;
}

void HexGame::printBoard()
{
	board.printBoard();
	cout << endl;
}

//getting the current player
Player HexGame::getPlayer()
{
	return player;
}

//checking if the game has ended or not
bool HexGame::gameEnd()
{
	if (board.winnerCheck(player)) {
		return true;
	}
	return false;
}

int main() {
	HexGame* g;
	//changing the board size, the board is default at 7
	int n = 7;
	cout << "Please choose the board size" << endl;
	cin >> n;
	g = new HexGame(n);

	cout << "Welcome to Hex Game" << endl;
	cout << endl;
	g->printBoard();

	cout << "Blue goes first!" << endl;
	cout << endl;

	//game will continue if the winning conditions has not been met
	while (!g->gameEnd()) {
		cout << "Player " << g->getPlayer() << "'s turn" << endl;
		cout << endl;
		cout << "Please choose row and column" << endl;
		int r, c;
		cin >> r >> c;

		if (g->moveIsValid(r, c)) {
			g->move(r, c);
		}
		else {
			cout << endl;
			cout << "Invalid move! Please input diffrent values" << endl;
			cout << endl;
		}
	}
	delete g;

	return 0;
}