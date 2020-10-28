CC=g++ -std=c++17
CFLAGS=-I$(IDIR) -lSDL2main -lSDL2 -lSDL2_image

IDIR =include
ODIR=obj
LDIR=lib

SRC=src

_DEPS = $(notdir $(shell find $(IDIR) -name '*.hpp'))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

SOURCES = $(notdir $(shell find $(SRC) -name '*.cpp'))

_OBJ = $(SOURCES:%.cpp=%.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o bin/$@ $^ $(CFLAGS)
	./bin/$@ ${framerate}

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

run {framerate}:
	./bin/main ${framerate}