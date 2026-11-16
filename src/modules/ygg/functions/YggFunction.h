#ifndef YGG_FUNCTION_H_
#define YGG_FUNCTION_H_
#ifdef HAVE_YGGINTERFACE
#include <YggInterface/YggInterface.hpp>
#include <yggdrasil_rapidjson/filereadstream.h>
#include <stdexcept>
#define MISSING_YGGINTERFACE_ERROR

namespace jags {
namespace ygg {

  class YggLoader
  {
  public:
    YggLoader(std::string const &name, unsigned int npar=1);
    YggLoader(const yggdrasil_rapidjson::Document& doc,
              std::string const &filename = "");
    std::string name;
    std::string jags_type;
    std::string filename;
    std::string address;
    yggdrasil_rapidjson::Document doc;
    unsigned int npar;
  protected:
    void _set_from_doc();
    static std::vector<unsigned int> _value2vector(const yggdrasil_rapidjson::Value& value);
  };

}
}

#else // NO HAVE_YGGINTERFACE
#define MISSING_YGGINTERFACE_ERROR \
  throw std::runtime_error("Cannot use the ygg module as JAGS was not build against the YggInterface library");
#endif // HAVE_YGGINTERFACE
#endif // YGG_FUNCTION_H_
