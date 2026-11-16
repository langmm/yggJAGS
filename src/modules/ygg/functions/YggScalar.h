#ifndef YGG_SCALAR_H_
#define YGG_SCALAR_H_

#include <function/ScalarFunction.h>
#include "YggFunction.h"

namespace jags {
namespace ygg {

#ifdef HAVE_YGGINTERFACE
  class YggScalarLoader : public YggLoader
  {
  public:
    YggScalarLoader(std::string const &name, unsigned int npar=1);
  };
#endif // HAVE_YGGINTERFACE

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
    YggInterface::communicator::Comm_t* comm;
  public:
    YggScalar(const YggScalarLoader& loader);
#endif
  };

}
}

#endif /* YGG_SCALAR_H_ */
