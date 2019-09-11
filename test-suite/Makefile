CC = gcc

CFLAGS = -Wall --coverage 
LFLAGS = -lcunit --coverage

DIR_SRC = src
DIR_OBJ	= obj
DIR_COV = res-coverage
DIR_UNIT = res-unit

DIR_COV_HTML = $(DIR_COV)/html

run: prepare $(DIR_OBJ)/main.o $(DIR_OBJ)/io.o 
	$(CC) -o $(DIR_OBJ)/test.out $(DIR_OBJ)/main.o $(DIR_OBJ)/io.o $(LFLAGS) -lm 
	./$(DIR_OBJ)/test.out
	lcov --capture --directory $(DIR_OBJ) --output-file $(DIR_COV)/coverage.info
	genhtml $(DIR_COV)/coverage.info --output-directory $(DIR_COV_HTML)

$(DIR_OBJ)/main.o: $(DIR_SRC)/main.c
	$(CC) -o $(DIR_OBJ)/main.o -c $(DIR_SRC)/main.c $(CFLAGS)

$(DIR_OBJ)/io.o: $(DIR_SRC)/io.c
	$(CC) -o $(DIR_OBJ)/io.o -c $(DIR_SRC)/io.c $(CFLAGS)

clean:
	rm -f -r $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)
	mkdir $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)

prepare:
	
	cp /usr/share/CUnit/*.xsl $(DIR_UNIT)