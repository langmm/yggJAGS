#ifndef YGG_VECTOR_H_
#define YGG_VECTOR_H_

#include <function/VectorFunction.h>
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
   * @short Call to model that results in a vector.
   * The YggVector function allows a set number of vector parameters to
   * be passed to a model that returns a vector.
   */
  class YggVector : public VectorFunction
  {
  public:
    YggVector(std::string const &name, unsigned int npar=1);
    ~YggVector();
    std::string deparse(std::vector<std::string> const &par) const;
    void evaluate(double *value, 
                  std::vector <double const *> const &args,
                  std::vector <unsigned int> const &lengths) const;
    unsigned int 
      length(std::vector <unsigned int> const &arglengths,
             std::vector <double const *> const &argvalues) const;

#ifdef HAVE_YGGINTERFACE
  private:
    YggInterface::YggRpcClient* comm;
#endif
  };

}
}

#endif /* YGG_VECTOR_H_ */
