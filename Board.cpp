#include "Board.h"




// It's helper function to recognition the correct chess piece. It creates the pointer to the new figure without create a prototype. 
void Board::set_figure(char name, std::vector<figure*>& row) {

	switch (name) {
	case 'x': {figure* tempfig = new figure; row.push_back(tempfig);  } break;
	case 'p': {pawn* temppawn = new pawn;  row.push_back(temppawn); } break;
	case 'w': {rook* temprook = new rook; row.push_back(temprook); } break;
	case 'h': {bishop* tempbishop = new bishop; row.push_back(tempbishop); } break;
	case 's': {knight* tempknight = new knight; row.push_back(tempknight); } break;
	case 'q': {queen* tempqueen = new queen; row.push_back(tempqueen); } break;
	case 'k': {king* tempking = new king; row.push_back(tempking); } break;
	default: row.push_back(nullptr); break;
	}

}

////////////////////////////////////////////////////////////// Constructor //////////////////////////////////////////////////////////////////////////////////////////

// in the constructor I gave the possibility to choose what board we want to play on.
// If you choose 0 you will play on the standard 8x8 board
Board::Board(bool m) : mat{ m } {

	bool turn_white = false;
	bool turn_black = false;

	int n = 0;
	std::cout << "what size is your board?" << std::endl;
	std::cout << "Standard: press 0, if unusal: write number of fields" << std::endl;
	std::cin >> n; std::cout << "\n";

	if (n == 0) {
		std::vector<figure*> row;
		board.reserve(8);
		for (uint32_t i = 0; i < 8; i++) {
			row.reserve(8);
			for (uint32_t j = 0; j < 8; j++) {
				try {
					char name = temp[i][j];
					set_figure(name, row);
					row.back()->figure_name = temp[i][j];
					row.back()->figure_color = tempColor[i][j];
				}
				catch (std::out_of_range) {
					std::cerr << "Smth went wrong, check your code" << std::endl;
				}

			}
			board.push_back(row);
			row.erase(row.begin(), row.end());
			row.shrink_to_fit();
		}

	}
	else {
		struct wrong_char { std::string error; };
		std::vector<figure*> row;
		board.reserve(n);
			char p, c;
			for (int32_t i = 0; i < n; i++) {
				row.reserve(n);
				for (int32_t k = 0; k < n; k++) {
					try { // handling an exception - in case of a mistake it iterates back by 1
						std::cout << "What is your figure in the place: " << i << ", " << k << " ?" << std::endl; std::cin >> p;
						std::cout << "What is color of your figure in the place: " << i << " ?" << std::endl; std::cin >> c;
						system("cls");
						if ((p == 'w' || p == 's' || p == 'h' || p == 'q' || p == 'k' || p == 'p' || p == 'x') && (c == 'w' || c == 'b' || c == 'x')) {
							set_figure(p, row);
							row.back()->figure_name = p;
							row.back()->figure_color = c;
						}
						else { wrong_char capsule; capsule.error = "This is not a figure"; throw capsule; }
					}
					catch (wrong_char& c) {
						std::cout << c.error << std::endl; k--;
					}
				}
				board.push_back(row);
				row.erase(row.begin(), row.end());
				row.shrink_to_fit();
			}
		}

}


/////////////////////////////////////////////////////////////////// Helper functions for component class figure ///////////////////////////////////////////////////////////////////////////////////


int conv(char l) {
	if (l >= 97 && l <= 122) l = l - 32;
	switch (l)
	{
	case 'A': return 1; break;
	case 'B': return 2; break;
	case 'C': return 3; break;
	case 'D': return 4; break;
	case 'E': return 5; break;
	case 'F': return 6; break;
	case 'G': return 7; break;
	case 'H': return 8; break;
	default: return 0;
	}
}

bool white_or_black(char dec) { if (dec == 'w') return true; else return false; }

/*
void set_beaten(Board::figure* f, Board::figure_list& beaten) {
	Board::figure* newfigure;
	newfigure = f->copy_figure(); /// use of  (pseudo) copy constructor
	/// checking the color of the figure and placing it in the right place:
	white_or_black(f->figure_color) ? beaten.push_front(newfigure) : beaten.push_back(newfigure);
	
}
*/

void set_beaten(Board::figure* f, std::multimap<char, Board::figure*>& lost_pieces) {
	Board::figure* newfigure;
	newfigure = f->copy_figure(); /// use of  (pseudo) copy constructor
	lost_pieces.insert({ newfigure->figure_name,newfigure });
}

//Nf = the figure you choose
//Ns = what is hidden in the place where you put the figure
//Sf = position on the board of the first figure
//Ss = position on the board of the second figure
//lf = char denoting the area of ​​the first figure
//ls = char denoting the area of ​​the second figure
//nf = number denoting the area of ​​the first figure
//ns = number denoting the area of ​​the second figure
//llf = the char denoting the area of ​​the first figure converted into a number
//lls = the char denoting the area of ​​the second figure converted into a number

struct transf {
	int Sf = 0, Ss = 0, iS = 0, jS = 0, iF = 0, jF = 0, llf = 0, lls = 0;

	transf(char lf, char ls, int nf, int ns) :
		Sf{ (nf * 8) - 8 + conv(lf) },
		Ss{ (ns * 8) - 8 + conv(ls) },
		iF{ nf - 1 },
		jF{ conv(lf) - 1 },
		iS{ ns - 1 },
		jS{ conv(ls) - 1 },
		llf{ conv(lf) },
		lls{ conv(ls) }{};
};

// mat checking function:
void mat_f(char color, bool& mat) {
	if (color == 'w')
		std::cout << "The white player wins!!" << std::endl;
	else if (color == 'b')
		std::cout << "The black player wins!!" << std::endl;
	mat = true;
}

// this algorithms prevent a situation in which another pawn is between the starting and ending square:
// this is the version for rook:
bool rook_check(int lf, int ls, int nf, int ns, Board::board_vector board, int iF, int jF) {
	int t;
	bool d, c = true;
	if (lf != ls) { t = (lf - ls); d = true; if (t > 0)  t--; else t++; }
	else if (nf != ns) { t = (nf - ns); d = false; if (t > 0) t--; else t++; }
	if (d == true) {
		while (c != false || t != 0) {
			if (t >= 0) { iF++; t--; }
			else { iF--; t++; }
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	}
	else if (d == false) {
		while (c != false || t != 0) {
			if (t >= 0) { jF++; t--; }
			else { jF++; t++; }
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	} 
	
	return c;
}



// this is the version for bishop:
bool bishop_check(int lf, int ls, int nf, int ns, Board::board_vector board, int iF, int jF) {
	int t, u;
	bool c = true;
	t = lf - ls; u = nf - ns;
	if (t >= 0 && u >= 0) {
		t--; u--;
		while (c != false || t != 0 || u != 0) {
			iF--; t--; jF--; u--;
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	}
	else if (t >= 0 && u < 0) {
		t--; u++;
		while (c != false || t != 0 || u != 0) {
			iF--; t--; jF++; u++;
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	}
	else if (t < 0 && u >= 0) {
		t++; u--;
		while (c != false || t != 0 || u != 0) {
			iF++; t++; jF--; u--;
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	}
	else if (t < 0 && u < 0) {
		t++; u++;
		while (c != false || t != 0 || u != 0) {
			iF++; t++; jF++; u++;
			board[iF][jF]->figure_name == 'x' ? c = true : c = false;
		}
	}
	return c;
}
// this is the combined version for the queen:
bool queen_check(int lf, int ls, int nf, int ns, Board::board_vector board, int iF, int jF) {
	if (lf == ls || nf == ns)  return rook_check(lf, ls, nf, ns, board, iF, jF);
	else if ((abs(ns - nf) == abs(ls - lf))) return bishop_check(lf, ls, nf, ns, board, iF, jF);
	else return false;
}

///////////////////////////////////////////////////////////////////////////// Class figure with child classes /////////////////////////////////////////////////////////////

void Board::pawn::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {

	transf Tr(lf, ls, nf, ns);

	char Nf = board[Tr.iF][Tr.jF]->figure_name;
	char Ns = board[Tr.iS][Tr.jS]->figure_name;



	if ((Nf == 'p' && Ns != 'x') && ((abs(nf - ns) == 1) && (abs(Tr.llf - Tr.lls) == 1)))
	{

		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten);
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if ((Nf == 'p' && Ns == 'x') && ((abs(Tr.Sf - Tr.Ss) == 8) || ((abs(Tr.Sf - Tr.Ss) == 16) && board[Tr.iS][Tr.jS]->m == 0)))
	{

		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl; turn = false; }
}

void Board::rook::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {
	
	transf Tr(lf, ls, nf, ns);

	char Nf = board[Tr.iF][Tr.jF]->figure_name;
	char Ns = board[Tr.iS][Tr.jS]->figure_name;


	if (!(lf == ls || nf == ns) || (rook_check(Tr.llf, Tr.lls, nf, ns, board, Tr.iF, Tr.jF) == false)){ std::cout << "Your move is incorrect!" << std::endl;  turn = false;
	}
	else if (Nf == 'w' && Ns != 'x')
	{   
		
		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten);
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if (Nf == 'w') {
		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl; turn = false;
	}

}

void Board::bishop::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {

	transf Tr(lf, ls, nf, ns);

	char Nf = board[Tr.iF][Tr.jF]->figure_name;
	char Ns = board[Tr.iS][Tr.jS]->figure_name;


	if (!(abs(ns - nf) == abs(Tr.lls - Tr.llf)) || bishop_check(Tr.llf, Tr.lls, nf, ns, board, Tr.iF, Tr.jF) == false) { std::cout << "Your move is incorrect!" << std::endl;  turn = false;
	}
	else if (Nf == 'h' && Ns != 'x')
	{
		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten);
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if (Nf == 'h' && Ns == 'x') {
		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl; turn = false;
	}
}

void Board::knight::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {

	transf Tr(lf, ls, nf, ns);

	char Nf = board[Tr.iF][Tr.jF]->figure_name;
	char Ns = board[Tr.iS][Tr.jS]->figure_name;


	if ((Nf == 's' && Ns != 'x') && (((nf == (ns + 2) || nf == (ns - 2)) &&
		(Tr.llf == (Tr.lls + 1) || Tr.llf == (Tr.lls - 1))) || ((Tr.llf == (Tr.lls + 2) || Tr.llf == (Tr.lls - 2)) &&
			(nf == ((ns + 1) | (nf == (ns - 1)))))))
	{
		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten);
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if (Nf == 's' && (((nf == (ns + 2) || nf == (ns - 2)) && 
		(Tr.llf == (Tr.lls + 1) || Tr.llf == (Tr.lls - 1))) || ((Tr.llf == (Tr.lls + 2) || Tr.llf == (Tr.lls - 2)) && 
			(nf == ((ns + 1) | (nf == (ns - 1)))))))
	{
		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl;  turn = false;
	}
}


void Board::queen::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {

	transf Tr(lf, ls, nf, ns);

	char Nf = board[Tr.iF][Tr.jF]->figure_name;
	char Ns = board[Tr.iS][Tr.jS]->figure_name;

	if ((Nf == 'q' && Ns != 'x') && (abs(ns - nf) == abs(Tr.lls - Tr.llf) || (Nf == 'q' && (Tr.llf == Tr.lls || nf == ns)) || bishop_check(Tr.llf, Tr.lls, nf, ns, board, Tr.iF, Tr.jF) == false))
	{ std::cout << "Your move is incorrect!" << std::endl; turn = false;
	}
	else if (Nf == 'q' && Ns != 'x')
	{
		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten);
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if (Nf == 'q')
	{
		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl;  turn = false;
	}
}


void Board::king::move(char lf, char ls, int nf, int ns, board_vector& board, memory_vector& memory_of_game, figure_list& beaten, bool& mat, bool& turn) {
	
	transf Tr(lf, ls, nf, ns); // it's transformation of variable

	char Nf = board[Tr.iF][Tr.jF]->figure_name; 
	char Ns = board[Tr.iS][Tr.jS]->figure_name;

	if ((Nf == 'k' && Ns != 'x') && ((abs(ns - nf) == 1) || (abs(Tr.lls - Tr.llf) == 1) || (abs(Tr.lls - Tr.llf) / 9 == 1)))
	{
		std::cout << "Player " << board[Tr.iS][Tr.jS]->figure_color << " loses " << board[Tr.iS][Tr.jS]->figure_name << "\n" << std::endl;
		set_beaten(board[Tr.iS][Tr.jS], beaten); // It's deep copy 
		if (Ns == 'k') mat_f(board[Tr.iF][Tr.jF]->figure_color, mat);
		figure* temp = new figure;
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';

		this->save_in_memory(board, memory_of_game);
	}
	else if (Nf == 'k' && ((abs(ns - nf) == 1) || (abs(Tr.lls - Tr.llf) == 1) || (abs(Tr.lls - Tr.llf) / 9 == 1)))
	{
		figure* temp = new figure;
		temp = board[Tr.iS][Tr.jS];
		board[Tr.iS][Tr.jS] = board[Tr.iF][Tr.jF];
		board[Tr.iF][Tr.jF] = temp;
		board[Tr.iF][Tr.jF]->figure_name = 'x';
		board[Tr.iF][Tr.jF]->figure_color = temp->figure_color;
		board[Tr.iS][Tr.jS]->counter();
		this->save_in_memory(board, memory_of_game);
	}
	else { std::cout << "Your move is incorrect!" << std::endl; turn = false;
	}
}




//////////////////////////////////////////////////////////////////////// Functions for class Board //////////////////////////////////////////////////////////////////////

// validation of the letter using ASCII:
bool check_sign(char lf, char ls, int nf, int ns) {
	if (lf >= 97 && lf <= 122) lf = lf - 32;
	if (ls >= 97 && ls <= 122) ls = ls - 32;
	if ((lf < 65 || lf > 74) || (ls < 65 || ls > 74)) return false;
	else return true;
}

// It,s my function to change the string to char and int:
struct conv_type {
	std::string Lf; std::string Ls; std::string Nf; std::string Ns;
	char lf, ls; int nf, ns;
	bool check = true;
	conv_type(std::string l1, std::string l2, std::string n1, std::string n2) :
		Lf{ l1 }, Ls{ l2 }, Nf{ n1 }, Ns{ n2 }{
		auto it = std::find_if(Lf.begin(), Lf.end(), [](auto i) { if ((i > 64 && i < 73) || (i > 96 && i < 105)) return true; });
		lf = static_cast<char>(*it);
		auto it2 = std::find_if(Ls.begin(), Ls.end(), [](auto i) { if ((i > 64 && i < 73) || (i > 96 && i < 105)) return true; });
		ls = static_cast<char>(*it2);
		auto it3 = std::find_if(Nf.begin(), Nf.end(), [](auto i) { if (i > 48 && i < 57) return true; });
		nf = static_cast<int>(*it3) - 48;
		auto it4 = std::find_if(Ns.begin(), Ns.end(), [](auto i) { if (i > 48 && i < 57) return true; });
		ns = static_cast<int>(*it4) - 48;
	}

};


// There are the functions that we can use the console controls.
//They are almost the same, but I have separated them for convenience
void Board::play_black() {
	char color = 'b';
	char color2 = 'w';
	std::string Nf; std::string Ns;
	std::string Lf; std::string Ls;
	this->turn = false;

		std::cout << "What is your move (black)?" << std::endl;
		std::cin  >> Nf; std::cout << " | \n"; std::cin >> Lf; std::cout << std::endl;
		std::cout << "Where put you a pawn? (black)?" << std::endl;
		std::cin  >> Ns; std::cout << " | \n"; std::cin >> Ls; std::cout << std::endl;
		conv_type test(Lf, Ls, Nf, Ns);

		if (check_sign(test.lf, test.ls, test.nf, test.ns) == true
			&& this->check_color(test.lf, test.nf, color, color2) == true)
		{
			this->turn = true;
			this->set_move(test.lf, test.ls, test.nf, test.ns);
		}
		else
		std::cout << "You wrote smth wrong!" << std::endl;
		
}

void Board::play_white() {
	char color = 'w';
	char color2 = 'b';
	std::string Nf; std::string Ns;
	std::string Lf; std::string Ls;
	this->turn = false;

		std::cout << "What is your move (white)?" << std::endl;
		std::cin >> Nf; std::cout << " | \n"; std::cin >> Lf; std::cout << std::endl;
		std::cout << "Where put you a pawn? (white)?" << std::endl;
		std::cin >> Ns; std::cout << " | \n"; std::cin >> Ls; std::cout << std::endl;
		conv_type test(Lf, Ls, Nf, Ns);

		if (check_sign(test.lf, test.ls, test.nf, test.ns) == true
			&& this->check_color(test.lf, test.nf, color, color2) == true)
		{
			this->turn = true;
			this->set_move(test.lf, test.ls, test.nf, test.ns);
			
		}
		else
		std::cout << "You wrote smth wrong!" << std::endl;

}

void disp_let(int n){
	char h = 65;
	std::cout << "  ";
	for (int i = 1; i < n; i++) {
		std::cout << static_cast<char>(h) << " ";
		h++;
	}
	std::cout << "\n";
}
// Show function. I created in main the overloaded function to display the board too
void Board::show() {
	 int n = 1;
	if (board.size() > 0) {
		std::cout << "      Black " << std::endl;
		for (auto& i : board) {
			std::cout << n << " "; n++;
			for (auto& j : i) {
				std::cout << j -> figure_name << " ";
			}
			std::cout << "\n";
		}
		disp_let(n);
		std::cout << "      White " << std::endl;
		std::cout << "\n";
	}
	else std::cout << "The board is empty" << std::endl;
}

// It shows lost pieces
/*
void Board::show_beaten() {
	if (beaten.size() > 0) {
		std::cout << "The lost figures:" << std::endl;
		for (auto it : beaten) {
			auto f = it;
			std::cout << f->figure_color << ": " << f->figure_name << std::endl;
		}
	}
	else std::cout << "The container is empty" << std::endl;
	//std::string('/', 100);
}
*/

void Board::show_beaten() {
	if (lost_pieces.size() > 0) {
		std::cout << "The lost figures:" << std::endl;
		for (auto& m : lost_pieces) {
			char col = m.first;
			auto f = m.second;
			std::cout << col << ": " << f->figure_name << std::endl;
		}
	}
	else std::cout << "The container is empty" << std::endl;
	
}

// It shows us the course of the game from the beginning to the end.
void Board::show_memory() {
	std::cout << "Your Game: " << std::endl << std::endl;
	if (this->memory_of_game.size() > 0) {
		for (auto& i : memory_of_game) {
			for (auto& j : i) {
				for (auto& k : j) {
					std::cout << k.figure_name << " ";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}
	else std::cout << "The container is empty" << std::endl;
}

bool Board::check_color(char lf, int nf, char col, char col2) {
	int i = nf - 1;
	int j = conv(lf) - 1;
	bool ret = true;
	this->board[i][j]->figure_color == col && col != col2 ? ret = true : ret = false;
	return ret;
}

// This is binding function beetwen class 
void Board::set_move(char lf, char ls, int nf, int ns) {
	int i = nf - 1;
	int j = conv(lf) - 1;
	this->board[i][j]->move(lf, ls, nf, ns, this->board, this->memory_of_game, this->lost_pieces, this->mat, this->turn);

};



//////////////////////////////////////////////////////////////////////// Whole game saving function  ////////////////////////////////////////////////////////////////


void Board::figure::save_in_memory(board_vector& board, memory_vector& memory_of_game) {
	std::size_t size = board[0].size();
	std::vector<figure> row;
	std::vector<std::vector<figure>> columns; columns.reserve(size);

	for (auto i = 0; i < board.size(); i++) {
		row.reserve(size);
		for (auto j = 0; j < board.size(); j++) {
			figure* newfigure;
			newfigure = board[i][j]->copy_figure();
			row.push_back(*newfigure);
		}
		columns.push_back(row);
		row.erase(row.begin(), row.end());
		row.shrink_to_fit();
	}
	size_t size_mem = memory_of_game.size();
	memory_of_game.push_back(columns);
	//auto it = back_inserter(memory_of_game);
	//auto it = columns; // It's possible to use an adaptor but there may be the compilation problems
	//Otherwise, adapters are designed to be used in  in iterative functions
}

//////////////////////////////////////////////////////////////////////// save and load functions - to the text file ////////////////////////////////////////////////////////////////

void Board::save() {
	std::ofstream save_in;
	save_in.open("chess.txt");
	int r{ 0 };
	for (auto& i : this -> memory_of_game) {
		for (auto& j : i) {
			for (auto& k : j) {
				save_in << k.figure_name;
			}
			save_in << std::endl;
		}
		save_in << "round: " << r++ << std::endl << std::endl;
	}
	save_in.close();
}

void Board::load() {
	std::ifstream loud_out;
	std::string temp;
	loud_out.open("chess.txt");
	while (!loud_out.eof()) {
		getline(loud_out, temp);
		std::cout << temp << std::endl;
	}
	loud_out.close();
}


////////////////////////////////////////////////////// delete of memory //////////////////////////////////////////////////////////////////////////////////
Board::~Board() {

	if (board.size() != 0 && board[0][0] != nullptr) {
		for (auto& i : board) {
			for (auto& j : i) {
				delete j;
			}
		}

	}
	if (lost_pieces.size() != 0) {
		for (auto& it : lost_pieces) {
			delete it.second;
		}
	}
}






