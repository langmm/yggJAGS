#include "testyggperf.h"
#include "testyggfunc.h"
#include "YggScalar.h"
#include "YggVector.h"
#include "YggArray.h"

#include <base/functions/Neg.h>
#include <bugs/functions/Inverse.h>
#include <bugs/functions/Sort.h>

#include <chrono>
#include <cstdlib>
#include <iostream>


namespace {

typedef std::chrono::steady_clock Clock;

unsigned int perf_iterations()
{
  char const *env = std::getenv("YGGPERF_ITERATIONS");
  if (!env) return 1000;
  unsigned long n = std::strtoul(env, nullptr, 10);
  return n > 0 ? static_cast<unsigned int>(n) : 1000;
}

double us_per_call(Clock::time_point const &start,
                   Clock::time_point const &end, unsigned int n)
{
  return std::chrono::duration<double, std::micro>(end - start).count() / n;
}

void report(char const *label, unsigned int n, double ygg_us, double base_us)
{
  std::cout << "[ PERF ] " << label << ": iterations=" << n
            << " ygg=" << ygg_us << " us/call"
            << " baseline=" << base_us << " us/call"
            << " ratio=" << (base_us > 0 ? ygg_us / base_us : 0.0) << "x"
            << std::endl;
}

unsigned int size_of(std::vector<unsigned int> const &dims)
{
  unsigned int size = 1;
  for (unsigned int d : dims) size *= d;
  return size;
}

} // namespace

void YggPerfTest::setUp()
{
#ifdef HAVE_YGGINTERFACE
  setenv(STRINGIFY(scalar_YGGJAGS_JSON), YGGTESTMODEL_JSON(scalar), 1);
  setenv(STRINGIFY(vector_YGGJAGS_JSON), YGGTESTMODEL_JSON(vector), 1);
  setenv(STRINGIFY(array_YGGJAGS_JSON), YGGTESTMODEL_JSON(array), 1);
  _scalar = new jags::ygg::YggScalar("scalar");
  _vector = new jags::ygg::YggVector("vector");
  _array = new jags::ygg::YggArray("array");
  _scalar_baseline = new jags::base::Neg;
  _vector_baseline = new jags::bugs::Sort;
  _array_baseline = new jags::bugs::Inverse;
#endif // HAVE_YGGINTERFACE
}

void YggPerfTest::tearDown()
{
#ifdef HAVE_YGGINTERFACE
  delete _scalar;
  delete _vector;
  delete _array;
  delete _scalar_baseline;
  delete _vector_baseline;
  delete _array_baseline;
  unsetenv(STRINGIFY(scalar_YGGJAGS_JSON));
  unsetenv(STRINGIFY(vector_YGGJAGS_JSON));
  unsetenv(STRINGIFY(array_YGGJAGS_JSON));
#endif // HAVE_YGGINTERFACE
}

#ifdef HAVE_YGGINTERFACE

void YggPerfTest::scalar()
{
  unsigned int n = perf_iterations();
  double x = 1.0;
  std::vector<double const *> args(1, &x);

  double ygg_value = _scalar->evaluate(args);
  double base_value = _scalar_baseline->evaluate(args);
  CPPUNIT_ASSERT_EQUAL(1.0, ygg_value);
  CPPUNIT_ASSERT_EQUAL(-1.0, base_value);

  double ygg_sum = 0.0, base_sum = 0.0;
  Clock::time_point start = Clock::now();
  for (unsigned int i = 0; i < n; ++i) ygg_sum += _scalar->evaluate(args);
  Clock::time_point mid = Clock::now();
  for (unsigned int i = 0; i < n; ++i) {
    base_sum += _scalar_baseline->evaluate(args);
  }
  Clock::time_point end = Clock::now();

  CPPUNIT_ASSERT_EQUAL(ygg_value * n, ygg_sum);
  CPPUNIT_ASSERT_EQUAL(base_value * n, base_sum);
  report("scalar", n, us_per_call(start, mid, n), us_per_call(mid, end, n));
}

void YggPerfTest::vector()
{
  unsigned int n = perf_iterations();
  double x[3] = {3, 1, 2};
  std::vector<double const *> args(1, x);
  std::vector<unsigned int> lengths(1, 3);

  std::vector<double> ygg_res(_vector->length(lengths, args));
  std::vector<double> base_res(_vector_baseline->length(lengths, args));

  _vector->evaluate(&ygg_res[0], args, lengths);
  _vector_baseline->evaluate(&base_res[0], args, lengths);
  double ygg_value = 0.0, base_value = 0.0;
  for (double v : ygg_res) ygg_value += v;
  for (double v : base_res) base_value += v;
  CPPUNIT_ASSERT_EQUAL(6.0, ygg_value);
  CPPUNIT_ASSERT_EQUAL(6.0, base_value);

  double ygg_sum = 0.0, base_sum = 0.0;
  Clock::time_point start = Clock::now();
  for (unsigned int i = 0; i < n; ++i) {
    _vector->evaluate(&ygg_res[0], args, lengths);
    ygg_sum += ygg_res[0];
  }
  Clock::time_point mid = Clock::now();
  for (unsigned int i = 0; i < n; ++i) {
    _vector_baseline->evaluate(&base_res[0], args, lengths);
    base_sum += base_res[0];
  }
  Clock::time_point end = Clock::now();

  CPPUNIT_ASSERT_EQUAL(ygg_res[0] * n, ygg_sum);
  CPPUNIT_ASSERT_EQUAL(base_res[0] * n, base_sum);
  report("vector", n, us_per_call(start, mid, n), us_per_call(mid, end, n));
}

void YggPerfTest::array()
{
  unsigned int n = perf_iterations();
  unsigned int d = 3;
  std::vector<double> A(d*d, 0);
  for (unsigned int i = 0; i < d; ++i) A[i + d*i] = 1;
  std::vector<double const *> args(1, &A[0]);
  std::vector<unsigned int> dA(2, d);
  std::vector<std::vector<unsigned int> > dims(1, dA);

  std::vector<double> ygg_res(size_of(_array->dim(dims, args)));
  std::vector<double> base_res(size_of(_array_baseline->dim(dims, args)));

  _array->evaluate(&ygg_res[0], args, dims);
  _array_baseline->evaluate(&base_res[0], args, dims);
  double ygg_value = 0.0, base_value = 0.0;
  for (double v : ygg_res) ygg_value += v;
  for (double v : base_res) base_value += v;
  CPPUNIT_ASSERT_EQUAL(3.0, ygg_value);
  CPPUNIT_ASSERT_EQUAL(3.0, base_value);

  double ygg_sum = 0.0, base_sum = 0.0;
  Clock::time_point start = Clock::now();
  for (unsigned int i = 0; i < n; ++i) {
    _array->evaluate(&ygg_res[0], args, dims);
    ygg_sum += ygg_res[0];
  }
  Clock::time_point mid = Clock::now();
  for (unsigned int i = 0; i < n; ++i) {
    _array_baseline->evaluate(&base_res[0], args, dims);
    base_sum += base_res[0];
  }
  Clock::time_point end = Clock::now();

  CPPUNIT_ASSERT_EQUAL(ygg_res[0] * n, ygg_sum);
  CPPUNIT_ASSERT_EQUAL(base_res[0] * n, base_sum);
  report("array", n, us_per_call(start, mid, n), us_per_call(mid, end, n));
}

#endif // HAVE_YGGINTERFACE
