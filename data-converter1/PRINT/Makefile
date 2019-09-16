CC = g++
DEBUG = -g

INCLUDE = -I$(DATACONVERTER_INCLUDE) -I/usr/local/xml/include/libxml2
CONTAINER = -std=c++0x
LIBS = -L/usr/local/xml/lib
CCFLAGS = -lxml2 -lm

OBJECTS = print_Vehicle.o print_Link.o print_ConnectionCell.o PRINT_Main.o

PRINT.o: $(OBJECTS)
	$(LD) -o $@ -r $(OBJECTS)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $*.cpp $(DEBUG) $(INCLUDE) $(CONTAINER)

clean: 
	rm -f *.o
