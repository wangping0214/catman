CXX = g++
FLAGS = -ggdb -Wall
OBJS = main.o
LIBS = -lpthread

main : main.o
	${CXX} ${FLAGS} -o $@ ${OBJS} ${LIBS}

mutex_demo : mutex_demo.o
	${CXX} ${FLAGS} -o $@ mutex_demo.o ${LIBS}

stack_management : stack_management.o
	${CXX} ${FLAGS} -o $@ stack_management.o ${LIBS}

main.o : main.cpp
	${CXX} ${FLAGS} -c main.cpp

mutex_demo.o : mutex_demo.cpp
	${CXX} ${FLAGS} -c mutex_demo.cpp

stack_management.o : stack_management.cpp
	${CXX} ${FLAGS} -c stack_management.cpp

*.o : *.cpp
	${CXX} ${FLAGS} -c $<

clean :
	rm -r ${OBJS}
