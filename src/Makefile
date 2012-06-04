
CXX=g++
FLAGS=-g -ggdb -pedantic -Wall
FLAGS= -O3 -pedantic -Wall

all: compiler app

compiler: compiler.cpp trie-dummy.hpp trie-fast.hpp
	${CXX} compiler.cpp -o compiler ${FLAGS}

app: app.cpp trie-dummy.hpp trie-fast.hpp
	${CXX} app.cpp -o app ${FLAGS}

test: compiler app
	./compiler ./testing.txt ./out.bin
	./app ./out.bin

check1: compiler app
	./compiler ./testing.txt ./out.bin
	./app ./out.bin > result
	diff result ./testing.txt

clean:
	rm -f compiler app
	rm -f *.bin
