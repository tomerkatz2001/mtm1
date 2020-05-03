//
// Created by bardv on 3/19/2020.
//

#include "test_utilities.h"
#include "map.h"
#include <stdlib.h>

#define NUMBER_TESTS 4

bool testMapCreateDestroy() {
    Map map = mapCreate();
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;

}

bool testMapAddAndSize() {
    Map map = mapCreate();
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map) == 1);
    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate();
    ASSERT_TEST(mapPut(map, "key", "value") == MAP_SUCCESS);
    char *getVal = mapGet(map, "key");
    ASSERT_TEST(strcmp(getVal, "value") == 0);
    mapDestroy(map);
    return true;
}

bool testIterator() {
    Map map = mapCreate();

    ASSERT_TEST(mapPut(map, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map, "key2", "value2") == MAP_SUCCESS);

    int i = 0;
    MAP_FOREACH(iter, map) {
        if (i != 2) {
            ASSERT_TEST(iter != NULL);
        } else {
            ASSERT_TEST(iter == NULL);
        }
        i++;
    }
    mapDestroy(map);
    return true;
}



bool (*tests[]) (void) = {
                      testMapCreateDestroy,
                      testMapAddAndSize,
                      testMapGet,
                      testIterator
};

const char* testNames[] = {
                           "testMapCreateDestroy",
                           "testMapAddAndSize",
                           "testMapGet",
                           "testIterator"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 1;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: map <test index>\n");
      return 2;
  }

  int test_idx = strtol(argv[1], NULL, 10);
  if (test_idx < 1 || test_idx > NUMBER_TESTS) {
      fprintf(stderr, "Invalid test index %d\n", test_idx);
      return 3;
  }

  RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
  return 4;
}