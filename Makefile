CPP = g++-10
#FLAGS = -std=c++11
LIB = -L/usr/local/Cellar/zeromq/4.3.3_1/lib
INC = -I/usr/local/Cellar/zeromq/4.3.3_1/include
ADDLIB = -lzmq
EXE = test
SRC = main.cpp

all:
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRC) $(ADDLIB) -o $(EXE)

clean:
	rm -f $(EXE)
