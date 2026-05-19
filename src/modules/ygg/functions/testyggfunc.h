#ifndef YGG_FUNC_TEST_H
#define YGG_FUNC_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <testlib.h>

class YggFuncTest : public CppUnit::TestFixture , public JAGSFixture
{
    CPPUNIT_TEST_SUITE( YggFuncTest );
    CPPUNIT_TEST( yggscalar );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
    void lgmix();
};

#endif  // YGG_FUNC_TEST_H


