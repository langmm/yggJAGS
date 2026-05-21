#include "YggArray.h"

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggArray::YggArray(std::string const &name, unsigned int npar) :
  ArrayFunction(name, npar), comm(nullptr) {
  comm = new YggInterface::YggRpcClient(name);
  if (npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"ndarray\","
                    "    \"subtype\": \"float\","
                    "  }"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"ndarray\","
                    "    \"subtype\": \"float\","
                    "  },"
                    "  \"minItems\": " + std::to_string(npar) + "," +
                    "  \"maxItems\": " + std::to_string(npar) +
                    "}", false, SEND);
  }
  comm->addSchema("{"
                  "  \"type\": \"ndarray\","
                  "  \"subtype\": \"float\""
                  "}", false, RECV);
}

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
  if (comm->call(request, response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error communicating with the model");
  }
  if (!response.IsNDArray()) {
    throw YggInterface::YggdrasilRuntimeError("Did not receive a array response");
  }
  std::vector <unsigned int> outdim = dim(dims, args);
  response.GetNDArray(value, outdim.data(), outdim.size());
}
std::vector<unsigned int> 
YggArray::dim(std::vector <std::vector<unsigned int> > const &dims,
              std::vector <double const *> const &values) const {
  std::vector<unsigned int> out;
  throw YggInterface::YggdrasilRuntimeError("NotImplemented");
  return out;
}
bool YggArray::checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) const {
  bool out = false;
  throw YggInterface::YggdrasilRuntimeError("NotImplemented");
  return out;
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
