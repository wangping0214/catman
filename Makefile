CXX = g++ -fPIC
FLAGS = -ggdb -Wall
#INCLUDES = -I/home/alan/workspace
INCLUDES = -I..
LIBS = -lpthread
CPPFILES = $(wildcard ./common/*.cpp) $(wildcard ./net/*.cpp) $(wildcard ./thread/*.cpp) $(wildcard ./tinyxml2/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
LIB = libcatman.so

all : $(LIB)

libcatman.so : $(OBJS)
	${CXX} -shared -o $(LIB) $(OBJS)

$(OBJS) : %.o : %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

.PHONY : clean
clean :
	-rm -r $(OBJS) $(LIB)

