#include "MyChess++.h"

using namespace std;


// Overloaded operator << for class Board:
std::ostream& operator<<(std::ostream& display, Board& game) {
	for (auto& i : game.board) {
		for (auto& j : i) {
			display << j -> figure_name;
			display << " ";
		}
		display << "\n";
	}
	display << "\n";
	return display;
};




int main()
{
	cout << "Welcome in Chess++" << endl;
	cout << "Do you want start?" << endl;
	cin.get();
	system("cls");

	Board my_game(false);
	int test = 0;

	my_game.show();

	// Here is the loop of the game:
	while (my_game.mat != true) {
		while (my_game.turn != true) {
			my_game.play_white();
		} 
		my_game.turn = false;
		while (my_game.turn != true) {
			my_game.play_black();
		}
		my_game.turn = false;
		cout << my_game;
		//cout << typeid(*my_game.board[0][0]).name() << endl;
		//cout << typeid(*my_game.board[1][0]).name() << endl;
		test++;
		//It's the test condition fo this loop:
		test == 1 ? my_game.mat = true : my_game.mat = false;
	}
	int s;
	cout << "save the game?\n" << "Yes [1]. No [0] " << endl;
	cin >> s; if (s != 0) { my_game.save(); }
	my_game.show_memory();
	my_game.show_beaten();

	cout << "Finish" << endl;
	cin.get();
	return 0;
}

