/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "stat_lib.h"
#include <vector>
using namespace std;


TEST_CASE("Successful Test Example") {
    int a = 5;
    CHECK(a == 5);
     vector<double> samples {1,2};
      CHECK(Average(samples) == 1.5);
      CHECK(doubleEqual(StandardDeviation(samples), 0.5));
      CHECK(doubleEqual(StandardDeviation(vector<double> {1,2,3,4,5,6,7}), 2));
      CHECK(doubleEqual(StandardDeviation(vector<double> {1,2,3,4,5,6,7,8}), 2.29128784755));
}
