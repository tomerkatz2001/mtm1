CC =gcc
OBJS= election.o map.o electionTestsExample.o
EXEC= election
DEBUG_FLAGS=
COMP_FLAGS= -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(COMP_FLAGS) $(OBJS) -o $@
election.o: election.c election.h mtm_map/map.h
	gcc -c $(DEBUG_FLAGS) $(COMP_FLAGS) election.c
electionTestsExample.o: tests/electionTestsExample.c tests/../election.h \
 tests/../mtm_map/map.h tests/../test_utilities.h
	gcc -c $(DEBUG_FLAGS) $(COMP_FLAGS) tests/electionTestsExample.c
map.o:  mtm_map/map.c mtm_map/map.h
	gcc -c $(DEBUG_FLAGS) $(COMP_FLAGS) mtm_map/map.c

clean:
	rm -f $(OBJS) $(EXEC)
