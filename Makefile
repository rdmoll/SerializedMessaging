CPP = g++
FLAGS = -std=c++11
LIB = -L/usr/local/lib
INC = -I/usr/local/include
ADDLIB = -lzmq -lprotoc -lprotobuf
EXE1 = test_1
EXE2 = test_2
SRC1 = main_1.cpp TableDefs.pb.cc
SRC2 = main_2.cpp TableDefs.pb.cc
PROTOFILE = TableDefs.proto

all:
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRC1) $(ADDLIB) -o $(EXE1)
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRC2) $(ADDLIB) -o $(EXE2)

protobuf:
	protoc -I=. --cpp_out=. $(PROTOFILE)

clean:
	rm -f $(EXEPUB)
	rm -f $(EXESUB)
	rm -f *.pb.cc *.pb.h
