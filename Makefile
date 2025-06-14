# our compiler
CC = gcc

# where to look for header files
INCLUDE = ./include

# compiler flags
CFLAGS = -I$(INCLUDE) -Wall -g -Werror

# linker flags
LDFLAGS = 

ARGS = 

# executable name
EXEC = main

# binary folder
BIN = ./bin

# source code folder
SRC = ./src

# all of our object files
OBJS = $(BIN)/main.o $(BIN)/signal.o $(BIN)/server.o $(BIN)/communication.o $(BIN)/routes.o

# rule to build executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# rule to build our object files
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

# run executable
run: $(EXEC)
	./$(EXEC) $(ARGS)

# clean all binary files
clean:
	rm -f $(OBJS) $(EXEC)

val: $(EXEC)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC) $(ARGS)