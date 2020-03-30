all: search-shell

search-shell: searcheng.cpp search-shell.cpp cmdhandler.cpp combiners.cpp handler.cpp md_parser.cpp searchui.cpp txt_parser.cpp util.cpp webpage.cpp
	g++ -std=c++11 -g -Wall searcheng.cpp search-shell.cpp cmdhandler.cpp combiners.cpp handler.cpp md_parser.cpp searchui.cpp txt_parser.cpp util.cpp webpage.cpp -o search-shell

crawl: crawl.cpp util.cpp searcheng.cpp md_parser.cpp txt_parser.cpp webpage.cpp
	g++ -g -Wall crawl.cpp util.cpp searcheng.cpp md_parser.cpp txt_parser.cpp webpage.cpp -o crawl

clean:
	rm -f *.o search-shell
