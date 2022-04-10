# CMakeList.txt: projekt CMake dla elementu MyChess++, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.

cmake_minimum_required (VERSION 3.8)

project ("MyChess++")

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (MyChess++ "MyChess++.cpp" "MyChess++.h" "Board.h" "Board.cpp")

