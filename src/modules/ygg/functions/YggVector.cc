#include "YggVector.h"

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggVector::YggVector(std::string const &name, unsigned int npar) :
  VectorFunction(name, npar), comm(nullptr) {
  comm = new YggInterface::YggRpcClient(name);
  if (npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"1darray\","
                    "    \"subtype\": \"float\","
                    "  }"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"1darray\","
                    "    \"subtype\": \"float\","
                    "  },"
                    "  \"minItems\": " + std::to_string(npar) + "," +
                    "  \"maxItems\": " + std::to_string(npar) +
                    "}", false, SEND);
  }
  comm->addSchema("{"
                  "  \"type\": \"1darray\","
                  "  \"subtype\": \"float\""
                  "}", false, RECV);
}

YggVector::~YggVector() {
  if (comm) {
    delete comm;
    comm = nullptr;
  }
}

std::string YggVector::deparse(std::vector<std::string> const &par) const
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

void YggVector::evaluate(double *value, 
                         std::vector <double const *> const &args,
                         std::vector <unsigned int> const &lengths) const {
  yggdrasil_rapidjson::Document request, response;
  request.SetArray();
  for (unsigned int i = 0; i < args.size(); ++i) {
    request.PushBack(yggdrasil_rapidjson::Value(args[i], lengths[i], request.GetAllocator()).Move(), request.GetAllocator());
  }
  if (comm->call(request, response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error communicating with the model");
  }
  if (!response.Is1DArray()) {
    throw YggInterface::YggdrasilRuntimeError("Did not receive a vector response");
  }
  response.Get1DArray(value, length(lengths, args));
}

unsigned int 
YggVector::length(std::vector <unsigned int> const &,
                  std::vector <double const *> const &) const {
  throw YggInterface::YggdrasilRuntimeError("NotImplemented");
  return 0;
}

#else // HAVE_YGGINTERFACE

// Stubs to allow compilation w/ error on use

YggVector::YggVector(std::string const &name, unsigned int npar) :
  VectorFunction(name, npar) {
  MISSING_YGGINTERFACE_ERROR
}
YggVector::~YggVector() {}
std::string YggVector::deparse(std::vector<std::string> const &) const { return ""; }
void YggVector::evaluate(double *, 
                         std::vector <double const *> const &,
                         std::vector <unsigned int> const &) const {}
unsigned int 
YggVector::length(std::vector <unsigned int> const &,
                  std::vector <double const *> const &) const { return 0; }

#endif // HAVE_YGGINTERFACE
