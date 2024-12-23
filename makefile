CXX = g++
CXXFLAGS = -Wall -g

test: mytest.o swarm.o
	$(CXX) $(CXXFLAGS) mytest.o swarm.o -o test

mytest.o: mytest.cpp swarm.h
	$(CXX) $(CXXFLAGS) -c mytest.cpp

snakes.o: swarm.cpp snakes.h
	$(CXX) $(CXXFLAGS) -c swarm.cpp

clean:
	rm -f *.o
	rm -f test
	rm -f *~

run: test
	./test

val: test
	valgrind ./test

val1:
	valgrind --track-origins=yes ./test

leak:
	valgrind --leak-check=full ./test



