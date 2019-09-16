CC = g++
DEBUG = -g

INCLUDE = -I$(DATACONVERTER_INCLUDE) -I/usr/local/xml/include/libxml2
CONTAINER = -std=c++0x
LIBS = -L/usr/local/xml/lib
CCFLAGS = -lxml2 -lm 

SUBDIRS = $(DATACONVERTER_LOAD) $(DATACONVERTER_CONVERT) $(DATACONVERTER_PRINT)

OBJECTS = $(DATACONVERTER_LOAD)/LOAD.o  $(DATACONVERTER_CONVERT)/CONVERT.o $(DATACONVERTER_PRINT)/PRINT.o
#OBJECTS = Link.o  Vehicle.o
#OBJECTS = Link.o Vehicle.o ConnectionCell.o


DataConverter: DATACONVERTER_Main.o
	for dir in $(SUBDIRS); do (cd $${dir}; $(MAKE)); done
	$(CC) -o $@ DATACONVERTER_Main.o $(OBJECTS) $(LIBS) $(CCFLAGS)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $*.cpp $(DEBUG) $(INCLUDE) $(CONTAINER)

clean: 
	for dir in $(SUBDIRS); do (cd $${dir}; $(MAKE) clean); done;
	rm -f *.o
	rm -f DataConverter
