#ifndef YGG_PERF_TEST_H
#define YGG_PERF_TEST_H

namespace jags {
    class ScalarFunction;
    class VectorFunction;
    class ArrayFunction;
}

#include <cppunit/extensions/HelperMacros.h>
#include <testlib.h>

class YggPerfTest : public CppUnit::TestFixture, public JAGSFixture
{
    CPPUNIT_TEST_SUITE( YggPerfTest );
#ifdef HAVE_YGGINTERFACE
    CPPUNIT_TEST( scalar );
    CPPUNIT_TEST( vector );
    CPPUNIT_TEST( array );
#endif
    CPPUNIT_TEST_SUITE_END();

#ifdef HAVE_YGGINTERFACE
    jags::ScalarFunction *_scalar;
    jags::ScalarFunction *_scalar_baseline;
    jags::VectorFunction *_vector;
    jags::VectorFunction *_vector_baseline;
    jags::ArrayFunction *_array;
    jags::ArrayFunction *_array_baseline;
#endif

public:
    void setUp();
    void tearDown();

#ifdef HAVE_YGGINTERFACE
    void scalar();
    void vector();
    void array();
#endif
};

#endif  // YGG_PERF_TEST_H
