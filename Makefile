CFLAGS=-lIce -lIceStorm -lIceUtil -pthread -lvlc
SRC=src/
INC=inc/
OBJ=obj/
BIN=bin/
ICE=ice/
CXX=clang++ -std=c++11 -I$(INC)

default: $(BIN)StreamServer.bin

all: slice default

iced: $(BIN)StreamServer.bin
	cd ice && icebox --Ice.Config=config.icestorm.icebox --daemon --nochdir

slice: $(ICE)Server.ice
	slice2cpp $< && mv Server.cpp $(SRC) && mv Server.h $(INC)

$(BIN)StreamServer.bin: $(OBJ)main.o $(OBJ)Server.o $(OBJ)StreamServer.o
	$(CXX) $(CFLAGS) $^ -o $@
$(OBJ)main.o: $(SRC)main.cpp
	$(CXX) -c $< -o $@
$(OBJ)Server.o: $(SRC)Server.cpp $(INC)Server.h
	$(CXX) -c $< -o $@
$(OBJ)StreamServer.o: $(SRC)StreamServer.cpp $(INC)StreamServer.hpp
	$(CXX) -c $< -o $@

clean:
	rm -f obj/*
