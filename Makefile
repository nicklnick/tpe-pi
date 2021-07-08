COMPILER=gcc
OUTPUT_FILE=imdb
FILES=main.c backEnd.c frontEnd.c imdbTAD.c
FLAGS=-Wall -pedantic -std=c99 -fsanitize=address
all:
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)
clean:
	rm $(OUTPUT_FILE)