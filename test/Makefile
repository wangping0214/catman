CXX = g++ 
FLAGS = -ggdb -Wall
INCLUDES = -I/home/alan/workspace
LIBS = ../libcatman.so -llog4cxx -lpthread
CPPFILES = $(wildcard *.cpp)
#OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
EXE = test

all : test Server Client OctetsTest

#$(EXE) : $(OBJS)
#	$(CXX) -o $(EXE) $(OBJS) $(LIBS)

test : main.o
	$(CXX) -o $@ $< $(LIBS)
main.o : main.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

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

OctetsTest : OctetsTest.o
	$(CXX) -o $@ $< $(LIBS)

OctetsTest.o : OctetsTest.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(OBJS) : %.o : %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

clean :
	rm -r $(wildcard *.o) test Server Client OctetsTest
#-rm -r $(OBJS) $(EXE) Server Client


