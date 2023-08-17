SRC   = $(wildcard source/*.c)
DEPS  = $(wildcard source/*.h)
OBJ   = $(addsuffix .o,$(subst source/,bin/,$(basename ${SRC})))
LIBS  = 
FLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -g
OUT   = yslvm

compile: ./bin $(OBJ) $(SRC) $(DEPS)
	$(CC) $(OBJ) $(LIBS) -o $(OUT)

./bin:
	mkdir -p bin

bin/%.o: source/%.c $(DEPS)
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm bin/*.o $(OUT)

