#include "testyggfunc.h"
#include "YggScalar.h"
#include "YggVector.h"
#include "YggArray.h"

#include <function/testfun.h>

#include <sstream>
#include <iostream>

using std::vector;
using std::string;
using std::stringstream;

#ifdef YGGTESTMODEL_PYTHON
#define YGGTESTMODEL_PYTHON_SETUP(type, cls)                            \
  setenv(STRINGIFY(type ## _OUT), YGGTESTMODEL_PYTHON(type), 1);        \
  setenv(STRINGIFY(type ## _OUT_COMM), YGGTESTMODEL_PYTHON(type), 1);   \
  _ ## type = new jags::ygg::cls(#type)
#define YGGTESTMODEL_PYTHON_TEARDOWN(type)      \
  unsetenv(STRINGIFY(type ## _OUT));               \
  unsetenv(STRINGIFY(type ## _OUT_COMM))
#else
#define YGGTESTMODEL_PYTHON_SETUP(type, cls)
#define YGGTESTMODEL_PYTHON_TEARDOWN(type)
#endif

void YggFuncTest::setUp()
{
#ifdef HAVE_YGGINTERFACE
  _scalar = nullptr;
  _vector = nullptr;
  _array = nullptr;
  YGGTESTMODEL_PYTHON_SETUP(scalar, YggScalar);
  // YGGTESTMODEL_PYTHON_SETUP(vector, YggVector);
  // YGGTESTMODEL_PYTHON_SETUP(array, YggArray);
#endif // HAVE_YGGINTERFACE
}

void YggFuncTest::tearDown()
{
#ifdef HAVE_YGGINTERFACE
  if (_scalar) delete _scalar;
  if (_vector) delete _vector;
  if (_array) delete _array;
  YGGTESTMODEL_PYTHON_TEARDOWN(scalar);
  // YGGTESTMODEL_PYTHON_TEARDOWN(vector);
  // YGGTESTMODEL_PYTHON_TEARDOWN(array);
#endif // HAVE_YGGINTERFACE
}

#ifdef HAVE_YGGINTERFACE

void YggFuncTest::invalid_model()
{
  CPPUNIT_ASSERT_THROW(jags::ygg::YggScalar("invalid"),
                       YggInterface::YggdrasilRuntimeError);
  CPPUNIT_ASSERT_THROW(jags::ygg::YggVector("invalid"),
                       YggInterface::YggdrasilRuntimeError);
  CPPUNIT_ASSERT_THROW(jags::ygg::YggArray("invalid"),
                       YggInterface::YggdrasilRuntimeError);
}

void YggFuncTest::npar()
{
  if (_scalar) CPPUNIT_ASSERT_EQUAL(_scalar->npar(), 1U);
  if (_vector) CPPUNIT_ASSERT_EQUAL(_vector->npar(), 1U);
  if (_array) CPPUNIT_ASSERT_EQUAL(_array->npar(), 1U);
}

void YggFuncTest::name()
{
  if (_scalar) CPPUNIT_ASSERT_EQUAL(string("scalar"), _scalar->name());
  if (_vector) CPPUNIT_ASSERT_EQUAL(string("vector"), _vector->name());
  if (_array) CPPUNIT_ASSERT_EQUAL(string("array"), _array->name());
}

void YggFuncTest::alias()
{
  if (_scalar) CPPUNIT_ASSERT_EQUAL(string(""), _scalar->alias());
  if (_vector) CPPUNIT_ASSERT_EQUAL(string(""), _vector->alias());
  if (_array) CPPUNIT_ASSERT_EQUAL(string(""), _array->alias());
}

void YggFuncTest::scalar()
{
  if(!_scalar) return;
  CPPUNIT_ASSERT_EQUAL(0.0, eval(_scalar, 0));
  CPPUNIT_ASSERT_EQUAL(1.0, eval(_scalar, 1));
}

void YggFuncTest::vector()
{
  if(!_vector) return;
  double v2[3] = {1, 2, 3};
  CPPUNIT_ASSERT_EQUAL(6.0, eval(_vector, v2));
}

void YggFuncTest::array()
{
  if(!_array) return;
  unsigned int n = 3;
  std::vector<double> A(n*n, 0);
  for (unsigned int i = 0; i < n; ++i) {
    A[i + n*i] = 1;
  }
  std::vector<double const *> argA(1, &A[0]);
  std::vector<unsigned int> dA(n, n);
  std::vector<std::vector<unsigned int> > dimA(1, dA);
  std::vector<double> resA(n*n);
  _array->checkParameterDim(dimA);
  _array->evaluate(&resA[0], argA, dimA);
  for (unsigned int i = 0; i < n; ++i) {
    CPPUNIT_ASSERT_DOUBLES_EQUAL(A[i], resA[i], tol);
  }
}

#else // HAVE_YGGINTERFACE

void YggFuncTest::missing()
{
  // TODO: Create specific error class
  CPPUNIT_ASSERT_THROW(jags::ygg::YggScalar("invalid"),
                       std::exception);
  CPPUNIT_ASSERT_THROW(jags::ygg::YggVector("invalid"),
                       std::exception);
  CPPUNIT_ASSERT_THROW(jags::ygg::YggArray("invalid"),
                       std::exception);
}

#endif // HAVE_YGGINTERFACE
