CC=g++
CXXFLAGS=-O3 -std=c++11 -Wall
PROJECT=Zork
FILES=$(PROJECT)  Makefile itemsIn.txt \
	src/main.cpp \
	src/BaseInformation.h src/Exceptions.h \
	src/Inventory.h src/Inventory.cc \
	src/Item.h src/Item.cc \
	src/Room.h src/Room.cc \
	src/Game.h src/Game.cc \

	
all: $(PROJECT)

$(PROJECT): main.o Room.o Item.o Inventory.o Game.o
	$(CC) $(CXXFLAGS) -o $@ $^

main.o: src/main.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

Room.o: src/Room.cc src/Room.h src/BaseInformation.h src/Item.h src/Exceptions.h
	$(CC) $(CXXFLAGS) -c -o $@ $<

Item.o: src/Item.cc src/Item.h src/BaseInformation.h
	$(CC) $(CXXFLAGS) -c -o $@ $<

Inventory.o: src/Inventory.cc src/Inventory.h src/BaseInformation.h src/Item.h src/Exceptions.h
	$(CC) $(CXXFLAGS) -c -o $@ $<

Game.o: src/Game.cc src/Game.h src/Room.h src/Inventory.h
	$(CC) $(CXXFLAGS) -c -o $@ $<

tar: $(PROJECT)
	tar -cvzf Zork_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT)
