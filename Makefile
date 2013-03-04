CXX = g++
FLAGS = -ggdb -Wall
INCLUDES = -I. -I./common -I./net
LIBS = -lpthread
CPPFILES = $(wildcard ./common/*.cpp) $(wildcard ./net/*.cpp)
OBJS = $(patsubst %.cpp,%.,$(CPPFILES))
LIB : libcatman.so

all : $(LIB)

libcatman.so : $(OBJS)
	${CXX} -fPIC --shared -o $(LIB) $(OBJS)

*.o : *.cpp
	${CXX} ${FLAGS} -c $<

clean :
	rm -r $(OBJS) $(LIB)
