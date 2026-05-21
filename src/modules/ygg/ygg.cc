#include <module/Module.h>
#include <functions/YggScalar.h>
#include <functions/YggVector.h>
#include <functions/YggArray.h>

// #ifdef HAVE_YGGINTERFACE
// #include <YggInterface/utils/regex.hpp>
// #endif

namespace jags {
namespace ygg {

  /**
   * The YggModule
   */
  class YggModule : public Module {
  public:
    YggModule();
    ~YggModule();
    void load();
  };

  YggModule::YggModule()
    : Module("ygg")
  {
#ifdef HAVE_YGGINTERFACE
    std::vector<std::string> model_types = {"SCALAR", "VECTOR", "ARRAY"};
    for (typename std::vector<std::string>::const_iterator model_type = model_types.begin();
         model_type != model_types.end(); model_type++) {
      std::vector<std::string> env_models;
      std::string env_var = "YGG_JAGS_" + (*model_type) + "_MODELS";
      char *raw_env_models = std::getenv(env_var.c_str());
      // TODO: Need to get npar from envvar?
      if (raw_env_models) {
        env_models = YggInterface::utils::split(raw_env_models, ",");
      }
      // TODO: Separate definitions for each type?
#ifdef YGGJAGS_MODELS
      std::string macro_models = YggInterface::utils::split(STRINGIFY(YGGJAGS_MODELS), ",");
      env_models.insert(env_models.end(),
                        macro_models.begin(), macro_models.end());
#endif
      for (typename std::vector<std::string>::const_iterator name = env_models.begin();
           name != env_models.end(); name++) {
        if ((*model_type) == "SCALAR") {
          insert(new YggScalar(*name));
        } else if ((*model_type) == "VECTOR") {
          insert(new YggVector(*name));
        } else if ((*model_type) == "ARRAY") {
          insert(new YggArray(*name));
        }
      }
    }
#endif
  }

  YggModule::~YggModule() {
    std::vector<Function*> const &fvec = functions();
    for (unsigned int i = 0; i < fvec.size(); ++i) {
      delete fvec[i];
    }
  }

  void YggModule::load() {
#ifdef HAVE_YGGINTERFACE
    MISSING_YGGINTERFACE_ERROR
#endif
    Module::load();
  }

}
}

// #endif /* MODULE_YGGDRASIL_H_ */
