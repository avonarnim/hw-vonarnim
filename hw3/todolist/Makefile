all: todolist-test

todolist-test: todolist.cpp todolist-test.cpp todolist.h
	g++ -std=c++11 -g -Wall todolist.cpp todolist-test.cpp -o todolist-test

clean:
	rm -f *.o todolist-test
