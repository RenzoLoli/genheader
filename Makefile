go: build run

test: 
	./genheader main.cpp && g++ test.cpp -o tester && ./tester && rm ./tester

build:
	g++ main.cpp -o genheader -I.

run:
	./genheader main.cpp
