#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <list>
#include <string>
#include <fstream>
#include <typeinfo>
#include <map>

class Board {

	// 	my board initialization vectors:
	std::vector<std::vector < char>> temp{ { 'w', 's', 'h', 'q', 'k', 'h', 's', 'w'},
	{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
	{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
	{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
	{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
	{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
	{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
	{'w', 's', 'h', 'q', 'k', 'h', 's', 'w' } };

	std::vector<std::vector<char>> tempColor{ {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'},
		{'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'},
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
		{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
		{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'} };






public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class figure;
	using board_vector = std::vector <std::vector<figure*>>;

	using memory_vector = std::vector <std::vector<std::vector<figure>>>;

	using figure_list = std::multimap<char, Board::figure*>;


//	for each inherited class, different "move" functions are used because each figure has different motion conditions
	class figure {
	public:
		char figure_name = 'x';
		char figure_color = 'x';
		int m = 0;
		int counter() { return m++; };
		figure* copy_figure() { return new figure(*this); } // It's kind of copy constructor (but it's not the constructor). it's allow the deep copy of object
		void save_in_memory(board_vector& board, memory_vector& memory_of_game);
		virtual void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) { };

	};

	class pawn : public figure {
	public:
		const char figure_name = 'p';
		figure* copy_figure() {
			return new pawn(*this);
		}
		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	class rook : public figure {
	public:
		const char figure_name = 'w';
		figure* copy_figure() {
			return new rook(*this);
		}
		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	class bishop : public figure {
	public:
		const char figure_name = 'h';
		figure* copy_figure() {
			return new bishop(*this);
		}
		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	class knight : public figure {
	public:
		const char figure_name = 's';
		figure* copy_figure() { 
			return new knight(*this);
		}
		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	class queen : public figure {
	public:
		const char figure_name = 'q';
		figure* copy_figure() {
			return new queen(*this);
		}
		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	class king : public figure {
	public:
		const char figure_name = 'k';
		figure* copy_figure() {
			return new king(*this);
		}

		void move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn);
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	board_vector board; // vector for every figures in the game
	memory_vector memory_of_game; // vector with all vectors, that represent each turn in the game
	figure_list lost_pieces; // multimap with every lost piece. It's ordered, it helps by unpacking
	bool mat = false; // if it's true the game is finisched 
	bool turn = false; // if it's true the turn is finished 
	Board(bool m); // constructor 
	~Board(); // destrucktor


	/*
	std::vector <std::vector<figure*>> board;
	std::vector <std::vector<std::vector<figure*>>> memory_of_game;
	std::list<figure*> beaten;
	//figure* operator= (figure* f);
	*/

	void set_move(char lf, char ls, int nf, int ns); // it's binding function beetwen class figure and Board to set and indirectly to check each move
	void play_white(); // turn of white player 
	void play_black(); // turn of black player
	void show(); // shows the board. I overloaded in main the operator << additionaly.
	void show_beaten(); // unpacking of lost_pieces
	void show_memory(); // unpacking of memory_of_game
	void set_figure(char name, std::vector<figure*>& row); // In this void I create the new figure 
	bool check_color(char lf, int nf, char col, char col2); // Check of color of piece
	void save(); // Saving to txt file 
	void load(); // loading from txt file

};
