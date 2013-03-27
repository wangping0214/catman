CXX = g++ 
FLAGS = -ggdb -Wall -DHAVE_CXX_STDHEADERS
DB_PATH = /usr/local/BerkeleyDB.5.3
INCLUDES = -I/home/alan/workspace -I./prodef -I$(DB_PATH)/include
LIBS = ../libcatman.so -llog4cxx -L$(DB_PATH)/lib -ldb_cxx -lpthread
#CPPFILES = $(wildcard *.cpp)
#OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
#EXE = test

all : Server Client catmandbd

#$(EXE) : $(OBJS)
#	$(CXX) -o $(EXE) $(OBJS) $(LIBS)

tinyxml2_test : tinyxml2_test.o
	$(CXX) -o $@ $< $(LIBS)

tinyxml2_test.o : tinyxml2_test.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

Server : Server.o
	$(CXX) -o $@ $< $(LIBS)

Server.o : Server.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

Client : Client.o
	$(CXX) -o $@ $< $(LIBS)

Client.o : Client.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

catmandbd : catmandbd.o
	$(CXX) -o $@ $< $(LIBS)

catmandbd.o : catmandbd.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

OctetsTest : OctetsTest.o
	$(CXX) -o $@ $< $(LIBS)

OctetsTest.o : OctetsTest.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

#$(OBJS) : %.o : %.cpp
#	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

clean :
	rm -r $(wildcard *.o) Server Client catmandbd
#-rm -r $(OBJS) $(EXE) Server Client


