all:
	g++ -I src/include -L src/lib -o Space Space.cpp -lmingw32 -lSDL2main -lSDL2