all: compile link 

compile:
	g++ -I src/include -c main.cpp -c src/Cell.cpp -c src/Field.cpp -c src/DrawText.cpp -c src/GetCell.cpp
link:
	g++ main.o Cell.o Field.o DrawText.o GetCell.o -o Minesweeper -L src/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio 