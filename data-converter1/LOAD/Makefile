CC = g++
DEBUG = -g

INCLUDE = -I$(DATACONVERTER_INCLUDE) -I/usr/local/xml/include/libxml2
CONTAINER = -std=c++0x
LIBS = -L/usr/local/xml/lib
CCFLAGS = -lxml2 -lm

OBJECTS = LOAD_Main.o load_Node.o load_Edge.o load_Net.o load_Route.o load_Connection.o load_TlLogic.o

LOAD.o: $(OBJECTS)
	$(LD) -o $@ -r $(OBJECTS)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $*.cpp $(DEBUG) $(INCLUDE) $(CONTAINER)

clean: 
	rm -f *.o
