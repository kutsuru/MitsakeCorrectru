CXX=g++
FLAGS=-g -ggdb -pedantic -Wall
FLAGS= -O3 -pedantic -Wall -std=c++0x

all: compiler app

compiler: src/compiler.cpp src/trie-dummy.hpp src/trie-fast.hpp
	${CXX} src/compiler.cpp -o ./compiler ${FLAGS}

app: src/app.cpp src/trie-dummy.hpp src/trie-fast.hpp
	${CXX} src/app.cpp -o app ${FLAGS}

test: compiler app
	./compiler ./testing.txt ./out.bin
	./app ./out.bin

doc:
	doxygen Doxyfile

gen:
	cd tests && ./generator.py *.txt

check:
	cd tests && ./moulette.py

time:
	cd tests && ./moulette.py -t


clean:
	rm -rf compiler app doc
	rm -f *.bin
