Adam von Arnim
vonarnim@usc.edu
SID: 9081417994

HW1

List of files: reversi.cpp, reversi.h, rational.cpp, rational.h, hw1.txt,
README.md, test-reversi.cpp, test-rational.cpp

Compiling:
Rational: g++ -g -Wall rational.cpp test-rational.cpp -o test-rational
Reversi: g++ -g -Wall reversi.cpp test-reversi.cpp -o test-reversi

Assumptions: In Reversi, I assumed that after saving a checkpoint, the player
who saved should still be given to lay down a tile. If undoing to a previous
checkpoint, I assumed that the player who was set to lay a tile at that
checkpoint would resume possession.
