CC = g++
DEBUG = -g

INCLUDE = -I$(DATACONVERTER_INCLUDE) -I/usr/local/xml/include/libxml2
CONTAINER = -std=c++0x
LIBS = -L/usr/local/xml/lib
CCFLAGS = -lxml2 -lm

OBJECTS = convert_ToVehicle.o convert_ToLink.o convert_ToConnectionCell.o CONVERT_Main.o

CONVERT.o: $(OBJECTS)
	$(LD) -o $@ -r $(OBJECTS)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $*.cpp $(DEBUG) $(INCLUDE) $(CONTAINER)

clean: 
	rm -f *.o
