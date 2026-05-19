#include "testygg.h"
#include "functions/testyggfunc.h"
#include <cppunit/extensions/HelperMacros.h>

void init_ygg_test() {
    CPPUNIT_TEST_SUITE_REGISTRATION( YggFuncTest );
}
