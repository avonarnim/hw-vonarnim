all: expr-tester

expr-tester: expr-ops.cpp expr-tester.cpp expr-reader.cpp
	g++ -g -Wall expr-ops.cpp expr-tester.cpp expr-reader.cpp -o expr-tester

clean:
	rm -f *.o expr-tester
