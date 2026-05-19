#ifndef MODULE_YGGDRASIL_H_
#define MODULE_YGGDRASIL_H_

#include <module/Module.h>
#include <functions/YggScalar.h>


namespace jags {
namespace ygg {

  /**
   * The YggModule
   */
  class YggModule : public Module {
  public:
    YggModule();
    ~YggModule();
  };

  YggModule::YggModule()
    : Module("ygg")
  {
    std::vector<std::string> scalar_models;
    char *raw_scalar_models = std::getenv("YGG_JAGS_SCALAR_MODELS");
    if (raw_models) {
      scalar_models = YggInterface::utils::split(raw_scalar_models, ",");
    }
    for (typename std::vector<std::string>::const_iterator name = scalar_models.begin();
         name != scalar_models.end(); name++) {
      insert(new YggScalar(name));
    }
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
