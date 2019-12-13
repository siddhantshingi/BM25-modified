IDIR = ./include
CC = g++
CFLAGS = -I$(IDIR)

# ODIR = ./build
EDIR = ./
SDIR = ./src


Indexer: $(SDIR)/main_indexing.cpp $(SDIR)/indexing.cpp $(SDIR)/stemming.cpp
	$(CC) $^ -o $@ $(CFLAGS)

bm25_run: ./src/main_bm25.cpp ./src/BM25.cpp ./src/indexing.cpp $(SDIR)/stemming.cpp 
	$(CC) $^ -o $@ $(CFLAGS)   

all:


.PHONY: clean

clean:
	rm -f ./Indexer ./bm25_run
