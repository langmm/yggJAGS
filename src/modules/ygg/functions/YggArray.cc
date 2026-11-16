#include "YggArray.h"

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggArrayLoader::YggArrayLoader(std::string const &name0,
                               unsigned int npar0) :
  YggLoader(name0, npar0), dim_res({1}), dim_par() {
  if (doc.IsObject()) {
    if (doc.HasMember("dim_res")) {
      dim_res.clear();
      dim_res = this->_value2vector(doc["dim_res"]);
    }
    if (doc.HasMember("dim_par")) {
      dim_par.clear();
      for (size_t i = 0; i < doc["dim_par"].Size(); i++) {
        std::vector<unsigned int> ivect = this->_value2vector(doc["dim_par"][i]);
        dim_par.push_back(ivect);
      }
      if (dim_par.size() != this->npar) {
        if (doc.HasMember("par"))
          throw YggInterface::YggdrasilRuntimeError("Length of dim_par (" + std::to_string(dim_par.size()) + ") does not match provided npar (" + std::to_string(this->npar) + ")");
        this->npar = dim_par.size();
      }
    }
  }
}

YggArray::YggArray(const YggArrayLoader& loader) :
  ArrayFunction(loader.name, loader.npar), comm(nullptr),
  exp_dim_res(loader.dim_res), exp_dim_par(loader.dim_par) {
  if (!loader.address.empty()) {
    comm = new YggInterface::communicator::FunctionComm(
      loader.name, loader.address,
      SEND, COMM_FLAG_INTERFACE);
  } else {
    comm = new YggInterface::YggFunction(loader.name);
  }
  if (!(comm && comm->valid()))
    throw YggInterface::YggdrasilRuntimeError("Invalid YggInterface comm \"" + loader.name + "\" (address = \"" + loader.address + "\")");
  if (!exp_dim_par.empty()) {
    std::string items_str = "";
    for (size_t i = 0; i < exp_dim_par.size(); i++) {
      if (i > 0) items_str += ",";
      items_str += "{\"type\": \"ndarray\", \"subtype\": \"float\", \"shape\": [" + std::to_string(exp_dim_par[i][0]);
      for (size_t j = 1; j < exp_dim_par[i].size(); j++) {
        items_str += ", " + std::to_string(exp_dim_par[i][j]);
      }
      items_str += "]}";
    }
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": [" + items_str + "]"
                    "}", false, SEND);
  } else if (loader.npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"ndarray\","
                    "    \"subtype\": \"float\""
                    "  }"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"ndarray\","
                    "    \"subtype\": \"float\""
                    "  },"
                    "  \"minItems\": " + std::to_string(loader.npar) + "," +
                    "  \"maxItems\": " + std::to_string(loader.npar) +
                    "}", false, SEND);
  }
  if (exp_dim_res.empty()) {
    comm->addSchema("{"
                    "  \"type\": \"ndarray\","
                    "  \"subtype\": \"float\""
                    "}", false, RECV);
  } else {
    std::string shape_str = std::to_string(exp_dim_res[0]);
    for (size_t i = 1; i < exp_dim_res.size(); i++) {
      shape_str += ", " + std::to_string(exp_dim_res[i]);
    }
    comm->addSchema("{"
                    "  \"type\": \"ndarray\","
                    "  \"subtype\": \"float\","
                    "  \"shape\": [" + shape_str + "]"
                    "}", false, RECV);
  }
}

YggArray::YggArray(std::string const &name, unsigned int npar) :
  YggArray(YggArrayLoader(name, npar)) {}

YggArray::~YggArray() {
  if (comm) {
    delete comm;
    comm = nullptr;
  }
}

std::string YggArray::deparse(std::vector<std::string> const &par) const
{
  std::string const &fname = this->name();
  std::string name;
  name.append(fname);
  name.append("(");
  for (unsigned int i = 0; i < par.size(); ++i) {
    if (i > 0)
      name.append(", ");
    name.append(par[i]);
  }
  name.append(")");
  return name;
}

void YggArray::evaluate(double *value, 
                        std::vector <double const *> const &args,
                        std::vector <std::vector<unsigned int> > const &dims) const {
  yggdrasil_rapidjson::Document request, response;
  request.SetArray();
  for (unsigned int i = 0; i < args.size(); ++i) {
    request.PushBack(yggdrasil_rapidjson::Value(args[i], dims[i].data(),
                                                dims[i].size(),
                                                request.GetAllocator()).Move(),
                     request.GetAllocator());
  }
  if (comm->send(request) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error sending request to the model");
  }
  if (comm->recv(response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error receiving response from the model");
  }
  if (!response.IsNDArray()) {
    throw YggInterface::YggdrasilRuntimeError("Did not receive a array response");
  }
  response.GetNDArray(value, exp_dim_res.data(), exp_dim_res.size());
}
std::vector<unsigned int> 
YggArray::dim(std::vector <std::vector<unsigned int> > const &dims,
              std::vector <double const *> const &values) const {
  return exp_dim_res;
}
bool YggArray::checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) const {
  if (exp_dim_par.empty())
    return true;
  return (dims == exp_dim_par);
}

#else // HAVE_YGGINTERFACE

// Stubs to allow compilation w/ error on use

YggArray::YggArray(std::string const &name, unsigned int npar) :
  ArrayFunction(name, npar) {
  MISSING_YGGINTERFACE_ERROR
}
YggArray::~YggArray() {}
std::string YggArray::deparse(std::vector<std::string> const &) const { return ""; }
void YggArray::evaluate(double *, 
                        std::vector <double const *> const &,
                        std::vector <std::vector<unsigned int> > const &) const {}
std::vector<unsigned int> 
YggArray::dim(std::vector <std::vector<unsigned int> > const &,
              std::vector <double const *> const &) const { return {}; }

bool YggArray::checkParameterDim(std::vector<std::vector<unsigned int> > const &) const { return false; }

#endif // HAVE_YGGINTERFACE
