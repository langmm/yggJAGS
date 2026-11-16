#ifndef YGG_FUNC_TEST_H
#define YGG_FUNC_TEST_H

namespace jags {
    class ScalarFunction;
    class VectorFunction;
    class ArrayFunction;
}

#include <cppunit/extensions/HelperMacros.h>
#include <testlib.h>
#include <function/testfun.h>

#define YGGTESTMODEL_JSON(type) STRINGIFY(/Users/langmm/JAGS/yggJAGS/example_models/type.json)
#define YGGTESTMODEL_PYTHON(type) STRINGIFY(python::/Users/langmm/JAGS/yggJAGS/example_models/example_models.py::type)

class YggFuncTest : public CppUnit::TestFixture , public JAGSFixture
{
    CPPUNIT_TEST_SUITE( YggFuncTest );
#ifdef HAVE_YGGINTERFACE
    CPPUNIT_TEST( invalid_model );
    CPPUNIT_TEST( npar );
    CPPUNIT_TEST( name );
    CPPUNIT_TEST( alias );
    CPPUNIT_TEST( scalar );
    CPPUNIT_TEST( vector );
    CPPUNIT_TEST( array );
#else
    CPPUNIT_TEST( missing );
#endif
    CPPUNIT_TEST_SUITE_END();

#ifdef HAVE_YGGINTERFACE
    jags::ScalarFunction *_scalar;
    jags::VectorFunction *_vector;
    jags::ArrayFunction *_array;
    jags::ArrayFunction *_array_to_array;
#endif

public:
    void setUp();
    void tearDown();
  
#ifdef HAVE_YGGINTERFACE
    void invalid_model();
    void npar();
    void name();
    void alias();
    void scalar();
    void vector();
    void array();
    void array_to_array();
#else
    void missing();
#endif
};

#endif  // YGG_FUNC_TEST_H


