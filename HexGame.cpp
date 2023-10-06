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

		// initialize an empty weight matrix between all the nodes
		for (int x = 0; x < totalNodes; x++) {
			vector<int> e;
			for (int y = 0; y < totalNodes; y++) {
				e.push_back(0);
			}
			weight.push_back(e);
		}
	}
	~Board();
	SquareVal getSquare(int i, int j);
	void setSquare(int i, int j, SquareVal v);
	int getNode(int i, int j);
	bool isWithinBoard(int i, int j);
	bool adjacent(int i, int j, int x, int y);
	void generateWeight(Player p);
	void computePaths();
	bool isReachable(int x, int y);
	void printBoard();
	void printWeightMatrix();

private:
	int size;
	int totalNodes;
	vector<vector<SquareVal>> board;
	vector<vector<int>> weight;
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
		|| (i == x - 1 && j == y - 1)
		|| (i == x + 1 && j == y + 1));
}

//add a weight to the edge between Node[i][j] and the rest of the board if it exists
void Board::generateWeight(Player p)
{
	//setting current value based on the current player
	auto currentVal = SquareVal::Blue;
	if (p == Player::Red) {
		currentVal = SquareVal::Red;
	}

	//going through the board and testing each square one by one, if they are adjacent  
	//and have the same SquareVal then there is an edge exists between them.
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int x = 0; x < size; x++) {
				for (int y = 0; y < size; y++) {
					if (adjacent(i, j, x, y) == true && getNode(i, j) != getNode(x, y) && board[i][j] == board[x][y]) {
						auto currentWeight = 0;
						if (board[i][j] == currentVal) {
							currentWeight = 1;
						}
						weight[getNode(i, j)][getNode(x, y)] = weight[getNode(x, y)][getNode(i, j)] = currentWeight;
					}
				}
			}
		}
	}
}

void Board::computePaths()
{
	//using Floyd Warshall algorithm is check if a path exists
	for (int k = 0; k < totalNodes; k++) {
		for (int i = 0; i < totalNodes; i++)
			for (int j = 0; j < totalNodes; j++)
				weight[i][j] = weight[i][j] | (weight[i][k] && weight[k][j]);
	}
}

//checking if node x can reach node y
bool Board::isReachable(int x, int y)
{
	if (weight[x][y] == 1)
		return true;
	else
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

//Print out weight matrix for checking
void Board::printWeightMatrix()
{
	for (int i = 0; i < totalNodes; i++) {
		for (int j = 0; j < totalNodes; j++)
			cout << weight[i][j];
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
	bool hasWon();

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
	board.generateWeight(player);
	board.computePaths();

	//print game board 
	cout << endl;
	board.printBoard();
	cout << endl;

	//printing the weight matrix for testing
	board.printWeightMatrix();
	cout << endl;

	//print out message when the current player win
	if (hasWon() == true) {
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

//checking if the player has won the game or not
bool HexGame::hasWon()
{
	//testing Blue's win condition: a path between 1st row and last row
	if (player == Player::Blue) {
		int i = 0;
		for (int j = 0; j < size; j++) {
			int x = size - 1;
			for (int y = 0; y < size; y++) {
				isWon = board.isReachable(board.getNode(i, j), board.getNode(x, y));
				if (isWon == true) {
					return true;
				}
				return false;
			}
		}
	}
	//testing Red's win condition: a path between 1st column and last column
	else {
		int j = 0;
		for (int i = 0; i < size; i++) {
			int y = size - 1;
			for (int x = 0; x < size; x++) {
				isWon = board.isReachable(board.getNode(i, j), board.getNode(x, y));
				if (isWon == true) {
					return true;
				}
				return false;
			}
		}
	}
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
	while (!g->hasWon()) {
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
	return 0;
}