CXX = g++ -fPIC
FLAGS = -ggdb -Wall
INCLUDES = -I. -I../common
LIBS = -lpthread
CPPFILES = $(wildcard ./common/*.cpp) $(wildcard ./net/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
LIB = libcatman.so

all : $(LIB)

libcatman.so : $(OBJS)
	${CXX} -shared -o $(LIB) $(OBJS)

*.o : *.cpp
	$(CXX) $(FLAGS) -c $<

clean :
	rm -r $(OBJS) $(LIB)
