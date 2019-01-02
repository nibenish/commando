CXX = gcc -Wall -g
LDFLAGS = -c
CXXFLAGS = -fPIC -o
OBJS = commando.o cmd.o cmdctl.o util.o

all : $(OBJS)
	$(CXX) $(CXXFLAGS) commando $(OBJS)

clean :
	rm -f *.o actual.txt expect.txt valgrind.txt

commando.o : commando.c commando.h
	$(CXX) $(LDFLAGS) $<

cmd.o : cmd.c commando.h
	$(CXX) $(LDFLAGS) $<

cmdctl.o : cmdctl.c commando.h
	$(CXX) $(LDFLAGS) $<

util.o : util.c commando.h
	$(CXX) $(LDFLAGS) $<
	
binary_tests.o : binary_tests.c commando.h tests.h 
	$(CC) -c $<

test_utils.o : test_utils.c tests.h
	$(CC) -c $<

binary_tests : binary_tests.o cmd.o cmdctl.o test_utils.o
	$(CC) -o $@ $^

test-binary : binary_tests
	valgrind ./binary_tests 

test-shell : commando
	./shell_tests.sh
