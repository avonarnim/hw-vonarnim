all: eventsim

eventsim: derived_events.cpp eventsim.cpp input_reader.cpp streetgrid.cpp
	g++ -std=c++11 -g -Wall derived_events.cpp eventsim.cpp input_reader.cpp streetgrid.cpp -o eventsim

clean:
	rm -f *.o eventsim
