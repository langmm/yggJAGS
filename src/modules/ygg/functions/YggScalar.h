#ifndef YGG_SCALAR_H_
#define YGG_SCALAR_H_

#include <function/ScalarFunction.h>
#ifdef HAVE_YGGINTERFACE
#include <YggInterface/YggInterface.hpp>

#include <stdexcept>

#define MISSING_YGGINTERFACE_ERROR
#else
// TODO: Use YggdrasilRuntimeError
#define MISSING_YGGINTERFACE_ERROR \
  throw std::runtime_error("Cannot use the ygg module as JAGS was not build against the YggInterface library");
#endif


namespace jags {
namespace ygg {

  /**
   * @short Call to model that results in a scalar.
   * The YggScalar function allows a set number of scalar parameters to
   * be passed to a model that returns a scalar.
   */
  class YggScalar : public ScalarFunction
  {
  public:
    YggScalar(std::string const &name, unsigned int npar=1);
    ~YggScalar();
    std::string deparse(std::vector<std::string> const &par) const;
    double evaluate(std::vector<double const *> const &args) const;
#ifdef HAVE_YGGINTERFACE
  private:
    YggInterface::YggRpcClient* comm;
#endif
  };

}
}

#endif /* YGG_SCALAR_H_ */
