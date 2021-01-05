CPP = g++
FLAGS = -std=c++11
LIB = -L/usr/local/lib
INC = -I/usr/local/include
ADDLIB = -lzmq -lprotoc -lprotobuf
EXEPUB = test_pub
EXESUB = test_sub
SRCPUB = main_pub.cpp TableDefs.pb.cc
SRCSUB = main_sub.cpp TableDefs.pb.cc
PROTOFILE = TableDefs.proto

all:
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRCPUB) $(ADDLIB) -o $(EXEPUB)
	$(CPP) $(FLAGS) $(LIB) $(INC) $(SRCSUB) $(ADDLIB) -o $(EXESUB)

protobuf:
	protoc -I=. --cpp_out=. $(PROTOFILE)

clean:
	rm -f $(EXEPUB)
	rm -f $(EXESUB)
	rm -f *.pb.cc *.pb.h
