CC = g++ -std=c++1z
FLAG = -Wall -Wextra -IIncludes
all:
	$(CC) $(FLAG) -o a.out Sources/main.cpp
deb:
	$(CC) -g3 $(FLAG) -o a.out Sources/main.cpp
run:
	./a.out test.some s
drun:
	gdb -tui -arg ./a.out test.some s
