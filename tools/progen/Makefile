CXX = g++
FLAGS = -ggdb -Wall
INCLUDES = -I../../../
LIBS = ../../libcatman.so -lpthread
CPPFILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(CPPFILES))
EXE = progen

all : $(EXE)

$(EXE) : $(OBJS)
	${CXX} -o $@ $(OBJS) $(LIBS)

$(OBJS) : %.o : %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

.PHONY : clean
clean :
	-rm -r $(OBJS) $(LIB)

