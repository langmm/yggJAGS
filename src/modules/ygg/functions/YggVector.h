#ifndef YGG_VECTOR_H_
#define YGG_VECTOR_H_

#include <function/VectorFunction.h>
#include "YggFunction.h"


namespace jags {
namespace ygg {

#ifdef HAVE_YGGINTERFACE
  class YggVectorLoader : public YggLoader
  {
  public:
    YggVectorLoader(std::string const &name, unsigned int npar=1);
    unsigned int length_res;
    std::vector<unsigned int> length_par;
  };
#endif // HAVE_YGGINTERFACE

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
    YggInterface::communicator::Comm_t* comm;
    unsigned int exp_length;  /*!< Expected length of returned vector. */
  public:
    YggVector(const YggVectorLoader& loader);
#endif
  };

}
}

#endif /* YGG_VECTOR_H_ */
