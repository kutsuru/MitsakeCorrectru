
all: compiler app

compiler: compiler.cpp trie-dummy.hpp trie-fast.hpp
	g++ compiler.cpp -o compiler

app: app.cpp trie-dummy.hpp trie-fast.hpp
	g++ app.cpp -o app

test: compiler app
	./compiler ./testing.txt ./out.bin
	./app ./out.bin
