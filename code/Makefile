CC = g++ 
SEQUENTIAL = sequential
OPENMPFLOYD = OpenMPFloyd
OPENMP2 = openmp2
BIN = bin
OBJ = obj
SRC = src
SRC_MP = $(SRC)/multiprocessor
CFLAGS =  -std=c++11 -fvisibility=hidden -lpthread -O3 -fopenmp -fexceptions

all: $(SEQUENTIAL) $(OPENMPFLOYD) $(OPENMP2)

$(SEQUENTIAL) : $(OBJ)/sequential.o $(OBJ)/Graph.o
	$(CC) $(CFLAGS) $(OBJ)/sequential.o $(OBJ)/Graph.o -o $(BIN)/$(SEQUENTIAL)

$(OPENMPFLOYD) : $(OBJ)/OpenMPFloyd.o $(OBJ)/Graph.o
	$(CC) $(CFLAGS) $(OBJ)/OpenMPFloyd.o $(OBJ)/Graph.o -o $(BIN)/$(OPENMPFLOYD)

$(OPENMP2) : $(OBJ)/openmp2.o $(OBJ)/Graph.o
	$(CC) $(CFLAGS) $(OBJ)/openmp2.o $(OBJ)/Graph.o -o $(BIN)/$(OPENMP2)

$(OBJ)/Graph.o : $(SRC)/Graph.cpp $(SRC)/Graph.h
	$(CC) $(CFLAGS) -c $(SRC)/Graph.cpp -o $(OBJ)/Graph.o

$(OBJ)/sequential.o : $(SRC_MP)/sequential.cpp $(SRC)/timing.h
	$(CC) $(CFLAGS) -c $(SRC_MP)/sequential.cpp -o $(OBJ)/sequential.o

$(OBJ)/OpenMPFloyd.o : $(SRC_MP)/OpenMPFloyd.cpp $(SRC)/timing.h
	$(CC) $(CFLAGS) -c $(SRC_MP)/OpenMPFloyd.cpp -o $(OBJ)/OpenMPFloyd.o

$(OBJ)/openmp2.o : $(SRC_MP)/openmp2.cpp $(SRC)/timing.h
	$(CC) $(CFLAGS) -c $(SRC_MP)/openmp2.cpp -o $(OBJ)/openmp2.o

clean:
	rm -f $(BIN)/*
	rm -rf $(OBJ)/*.o