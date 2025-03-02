SRC_DIR = ./

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

LIB_NAME = mylib

TARGET = pr2.out

all: $(TARGET)

$(TARGET): main.o lib$(LIB_NAME).a
	gcc -o $(TARGET) main.o -L. -l$(LIB_NAME)

lib$(LIB_NAME).a: $(OBJ)
	ar rcs lib$(LIB_NAME).a $(OBJ)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -c $< -o $@

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f $(OBJ) lib$(LIB_NAME).a $(TARGET) main.o