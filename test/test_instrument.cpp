#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "instrument.h"

#define PREC .0001


TEST(test_instrument){

    // test constructor and the relational operators that are required by std::map
    Instrument a("ABC");
    Instrument b("XYZ");
    CHECK(a < b);
    CHECK(a == a);

}
