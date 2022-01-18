PROGRAM = Assignment4
OBJ = regions.o list.o main.o

$(PROGRAM): $(OBJ)
	clang -Wall $(OBJ) -o $(PROGRAM) -DNDEBUG
main.o: main.c regions.o
	clang -Wall -c main.c -o main.o -DNDEBUG

list.o: list.c list.h blocks.h
	clang -Wall -c list.c -o list.o -DNDEBUG

regions.o: regions.c list.o regions.h list.h blocks.h
	clang -Wall -c regions.c -o regions.o -DNDEBUG

clean:
	rm  -f $(PROGRAM) $(OBJ)