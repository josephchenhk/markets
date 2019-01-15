#include <UnitTest++/UnitTest++.h>
#include <Eigen/Dense>

#include "instrument.h"

#define PREC .0001


TEST(test_instrument){

    // test constructor and the relational operators that are required by std::map
    Instrument a("ABC");
    Instrument b("WXYZ");
    CHECK(a < b);
    CHECK(a == a);

    Instrument c("XLP");
    Instrument d("XLRE");
    Instrument e("XLU");
    CHECK(c < d);
    CHECK(d < e);
    CHECK(c < e);
    CHECK(c == c);
    CHECK(d == d);
    CHECK(e == e);

    Instrument a1("XLB");
    Instrument a2("XLE");
    Instrument a3("XLF");
    Instrument a4("XLI");
    Instrument a5("XLK");
    Instrument a6("XLV");
    Instrument a7("XLY");
    CHECK(a1 < a2);
    CHECK(a2 < a3);
    CHECK(a4 < a5);
    CHECK(a5 < a6);
    CHECK(a6 < a7);    


    // TODO: does equality work when they are different objects with the same ticker?
    //
}
