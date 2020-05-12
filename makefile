FLAGS= -std=c99 election -Wall -pedantic-errors -Werror -DNDEBUG

election: election.o electionTestsExample.o map.o 
	gcc election.o electionTestsExample.o map.o -o prog
election.o: election.c election.h mtm_map/map.h
	gcc -c $(FLAGS) election.c
electionTestsExample.o: tests/electionTestsExample.c tests/../election.h \
 tests/../mtm_map/map.h tests/../test_utilities.h
	gcc -c $(FLAGS) tests/electionTestsExample.c
map.o: mtm_map/map.c mtm_map/map.h
	gcc -c $(FLAGS) mtm_map/map.c

clean:
	rm -f election.o electionTestsExample.o map.o prog
