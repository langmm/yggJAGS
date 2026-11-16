#include <filesystem>
#include "YggFunction.h"

#ifdef HAVE_YGGINTERFACE

using std::vector;
using std::string;

using namespace jags::ygg;

YggLoader::YggLoader(std::string const &name0,
                     unsigned int npar0) :
  name(name0), jags_type(), filename(), address(), doc(), npar(npar0) {
  if (name.size() > 5 && (name.compare(name.size() - 5, 5, ".json") == 0)) {
    filename = name;
  } else {
    std::string kname = name + "_YGGJAGS_JSON";
    std::string filename_env = YggInterface::utils::getenv(kname);
    if (!filename_env.empty())
      filename = filename_env;
  }
  if (!filename.empty()) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if(!fp) {
      throw YggInterface::YggdrasilRuntimeError("Failed to read function parameters from \"" + filename + "\"");
    }
    char readBuffer[1024];
    yggdrasil_rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(is);
    fclose(fp);
  }
  _set_from_doc();
}
YggLoader::YggLoader(const yggdrasil_rapidjson::Document& doc0,
                     std::string const &filename0) :
  name(), jags_type(), filename(filename0), address(), doc(), npar(0) {
  doc.CopyFrom(doc0, doc.GetAllocator(), true);
  _set_from_doc();
}
void YggLoader::_set_from_doc() {
  if (doc.IsObject()) {
    if (doc.HasMember("jags_type"))
      jags_type.assign(doc["jags_type"].GetString(),
                       doc["jags_type"].GetStringLength());
    if (doc.HasMember("source"))
      filename.assign(doc["source"].GetString(),
                      doc["source"].GetStringLength());
    if (doc.HasMember("name"))
      name.assign(doc["name"].GetString(), doc["name"].GetStringLength());
    if (doc.HasMember("address")) {
      address.assign(doc["address"].GetString(),
                     doc["address"].GetStringLength());
      size_t idx = address.find("::");
      if (idx != std::string::npos) {
        size_t idx_second = address.find("::", idx + 2);
        if (idx_second != std::string::npos) {
          if (name.empty()) {
            name = address.substr(idx_second + 2);
          }
          std::filesystem::path path_address(address.begin() + idx + 2,
                                             address.begin() + idx_second);
          if (idx_second != std::string::npos && path_address.is_relative()) {
            std::filesystem::path path_json(filename);
            std::filesystem::path abspath = path_json.parent_path() / path_address;
            address.replace(idx + 2, idx_second - idx - 2, abspath);
          }
        }
      }
    }
    if (doc.HasMember("npar"))
      npar = doc["npar"].GetUint();
  }
}

std::vector<unsigned int> YggLoader::_value2vector(const yggdrasil_rapidjson::Value& value) {
  std::vector<unsigned int> out;
  if (!value.IsArray())
    throw YggInterface::YggdrasilRuntimeError("value is not an array");
  if (value.Size() == 0)
    return out;
  for (size_t i = 0; i < value.Size(); i++) {
    if (value[i].IsUint()) {
      out.push_back(value[i].GetUint());
    } else if (value[i].IsInt() && value[i].GetInt() >= 0) {
      out.push_back(static_cast<unsigned int>(value[i].GetInt()));
    } else {
      throw YggInterface::YggdrasilRuntimeError("Element " + std::to_string(i) + " is not uint");
    }
  }
  return out;
}

#endif // HAVE_YGGINTERFACE
