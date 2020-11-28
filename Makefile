CC=g++ -Wall
CFLAGS=-I$(IDIR) -lSDL2main -lSDL2 -lSDL2_image

IDIR =include
ODIR=obj
BDIR=bin
LDIR=lib

SRC=src

_DEPS = $(notdir $(shell find $(IDIR) -name '*.hpp'))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

SOURCES = $(notdir $(shell find $(SRC) -name '*.cpp'))

_OBJ = $(SOURCES:%.cpp=%.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: all

all: $(ODIR) $(BDIR) main

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o bin/$@ $^ $(CFLAGS)
	./bin/$@ ${framerate}

$(ODIR):
	mkdir -p $(ODIR)

$(BDIR):
	mkdir -p $(BDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

run {framerate}:
	./bin/main ${framerate}
