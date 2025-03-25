# Makefile to compile the election application
# Authored by Zoe Sepersky

# compile using make, then by using ./election_app
CXX=g++
CXXFLAGS	=	-std=c++11	-g
.DEFAULT_GOAL	:=	all
all:	election_app
# edit to add stv/plurality election classes later
election_app:	Election.o	ballot.o	candidate.o	pluralityballot.o	stvballot.o	userinterface.o
	$(CXX)	$(CXXFLAGS)	Election.o	ballot.o	candidate.o	pluralityballot.o	stvballot.o	userinterface.o	-o	election_app
Election.o:
	$(CXX)	$(CXXFLAGS) -c Election.cpp -o	Election.o
ballot.o:
	$(CXX)	$(CXXFLAGS) -c ballot.cpp -o	ballot.o
candidate.o:
	$(CXX)	$(CXXFLAGS) -c candidate.cpp -o	candidate.o
pluralityballot.o:
	$(CXX)	$(CXXFLAGS) -c pluralityballot.cpp -o	pluralityballot.o
stvballot.o:
	$(CXX)	$(CXXFLAGS) -c stvballot.cpp -o	stvballot.o
userinterface.o:
	$(CXX)	$(CXXFLAGS) -c userinterface.cpp -o	userinterface.o

.PHONY:	all	clean

clean:
	rm	-f	*.o	election_app

# to generate doxy files, use make docs

docs:
	cd ../documentation; doxygen Doxyfile; cd ../src;