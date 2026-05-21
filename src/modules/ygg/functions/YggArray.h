#ifndef YGG_ARRAY_H_
#define YGG_ARRAY_H_

#include <function/ArrayFunction.h>
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
   * @short Call to model that results in a array.
   * The YggArray function allows a set number of array parameters to
   * be passed to a model that returns a array.
   */
  class YggArray : public ArrayFunction
  {
  public:
    YggArray(std::string const &name, unsigned int npar=1);
    ~YggArray();
    std::string deparse(std::vector<std::string> const &par) const;
    void evaluate(double *value, 
                  std::vector <double const *> const &args,
                  std::vector <std::vector<unsigned int> > const &dims) const;
    std::vector<unsigned int> 
      dim(std::vector <std::vector<unsigned int> > const &dims,
          std::vector <double const *> const &values) const;
    bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) const;
#ifdef HAVE_YGGINTERFACE
  private:
    YggInterface::YggRpcClient* comm;
#endif
  };

}
}

#endif /* YGG_ARRAY_H_ */
