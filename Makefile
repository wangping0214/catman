CXX = g++ -fPIC
FLAGS = -ggdb -Wall -DHAVE_CXX_STDHEADERS
#INCLUDES = -I/home/alan/workspace
DB_PATH = /usr/local/BerkeleyDB.5.3
INCLUDES = -I.. -I$(DB_PATH)/include
LIBS = -L$(DB_PATH)/lib -ldb -lpthread 
CPPFILES = $(wildcard ./common/*.cpp) $(wildcard ./net/*.cpp) $(wildcard ./thread/*.cpp) $(wildcard ./db/*.cpp) \
		   $(wildcard ./tinyxml2/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
LIB = libcatman.so

all : $(LIB)

libcatman.so : $(OBJS)
	${CXX} -shared -o $(LIB) $(OBJS) $(LIBS)

$(OBJS) : %.o : %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

.PHONY : clean
clean :
	-rm -r $(OBJS) $(LIB)

