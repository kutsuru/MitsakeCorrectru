CXX=g++
FLAGS=-g -ggdb -pedantic -Wall -std=c++0x -DDEBUG_DIST=1 -DDEBUG_DEL=1
#FLAGS= -O3 -pedantic -Wall -std=c++0x

all: TextMiningCompiler TextMiningApp

TextMiningCompiler: src/compiler.cpp src/trie-dummy.hpp src/trie-fast.hpp
	${CXX} src/compiler.cpp -o ./TextMiningCompiler ${FLAGS}

TextMiningApp: src/app.cpp src/trie-dummy.hpp src/trie-fast.hpp
	${CXX} src/app.cpp -o ./TextMiningApp ${FLAGS}

test: TextMiningCompiler TextMiningApp
	./TextMiningCompiler ./testing.txt ./out.bin
	./TextMiningApp ./out.bin

doc:
	doxygen Doxyfile

gen:
	cd tests && ./generator.py *.txt

check:
	cd tests && ./moulette.py

time:
	cd tests && ./moulette.py -t


clean:
	rm -rf doc TextMiningCompiler TextMiningApp
	rm -f *.bin
