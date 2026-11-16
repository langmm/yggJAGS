#ifndef YGG_ARRAY_H_
#define YGG_ARRAY_H_

#include <function/ArrayFunction.h>
#include "YggFunction.h"

namespace jags {
namespace ygg {

#ifdef HAVE_YGGINTERFACE
  class YggArrayLoader : public YggLoader
  {
  public:
    YggArrayLoader(std::string const &name, unsigned int npar=1);
    std::vector<unsigned int> dim_res;
    std::vector< std::vector<unsigned int> > dim_par;
  };
#endif // HAVE_YGGINTERFACE

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
    YggInterface::communicator::Comm_t* comm;
    std::vector<unsigned int> exp_dim_res;  /*!< Expected dimensions of returned array. */
    std::vector< std::vector<unsigned int> > exp_dim_par;  /*!< Expected dimensions of parameters */
  public:
    YggArray(const YggArrayLoader& loader);
#endif
  };

}
}

#endif /* YGG_ARRAY_H_ */
