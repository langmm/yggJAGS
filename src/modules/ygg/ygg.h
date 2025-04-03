#ifndef MODULE_YGGDRASIL_H_
#define MODULE_YGGDRASIL_H_

#include <function/ScalarFunction.h>
#include <module/Module.h>
#include <YggInterface.hpp>


namespace jags {
namespace ygg {

  /**
   * @short Call to model that results in a scalar.
   * The YggScalarModel function implements 
   */
  class YggScalarModel : public ScalarFunction
  {
  public:
    YggScalarModel(std::string const &name, unsigned int npar);
    std::string deparse(std::vector<std::string> const &par) const;
    double evaluate(std::vector<double const *> const &args) const;
    bool checkParameterValue(std::vector<double const *> const &args) const;
  private:
    yggdrasil::YggRpcClient* comm;
  };

  /**
   * The YggModule function
   */
  class YggModule : public Module {
    YggModule();
    ~YggModule();
  };

  YggModule::YggModule()
    : Module("ygg")
  {
    std::string name = 
    insert(new YggScalarModel);
  }

  YggModule::~YggModule() {
    vector<Function*> const &fvec = functions();
    for (unsigned int i = 0; i < fvec.size(); ++i) {
      delete fvec[i];
    }
    
  }

}
}

#endif /* MODULE_YGGDRASIL_H_ */
