CPP = g++
FLAGS = -std=c++11
LIB = -L/usr/local/lib
INC = -I/usr/local/include
ADDLIB = -lzmq
EXEPUB = test_pub
EXESUB = test_sub
SRCPUB = main_pub.cpp
SRCSUB = main_sub.cpp

all:
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRCPUB) $(ADDLIB) -o $(EXEPUB)
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRCSUB) $(ADDLIB) -o $(EXESUB)

clean:
	rm -f $(EXEPUB)
	rm -f $(EXESUB)
