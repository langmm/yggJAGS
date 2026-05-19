#ifndef YGG_SCALAR_H_
#define YGG_SCALAR_H_

#include <function/ScalarFunction.h>
#include <YggInterface.hpp>


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
    YggScalar(std::string const &name, unsigned int npar=0);
    std::string deparse(std::vector<std::string> const &par) const;
    double evaluate(std::vector<double const *> const &args) const;
    bool checkParameterValue(std::vector<double const *> const &args) const;
  private:
    yggdrasil::YggRpcClient* comm;
  };

}
}

#endif /* YGG_SCALAR_H_ */
