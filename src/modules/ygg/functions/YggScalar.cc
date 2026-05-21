#include "YggScalar.h"

using std::vector;
using std::string;

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggScalar::YggScalar(string const &name, unsigned int npar) :
  ScalarFunction(name, npar), comm(nullptr) {
  comm = new YggInterface::YggRpcClient(name);
  if (npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"}"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"},"
                    "  \"minItems\": " + std::to_string(npar) + "," +
                    "  \"maxItems\": " + std::to_string(npar) +
                    "}", false, SEND);
  }
  comm->addSchema("{"
                  "  \"type\": \"number\""
                  "}", false, RECV);
}

YggScalar::~YggScalar() {
  if (comm) {
    delete comm;
    comm = nullptr;
  }
}

string YggScalar::deparse(vector<string> const &par) const
{
  string const &fname = this->name();
  string name;
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

double YggScalar::evaluate(vector <double const *> const &args) const {
  yggdrasil_rapidjson::Document request, response;
  request.SetArray();
  for (unsigned int i = 0; i < args.size(); ++i) {
    request.PushBack(yggdrasil_rapidjson::Value(*args[i]).Move(), request.GetAllocator());
  }
  if (comm->call(request, response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error communicating with the model");
  }
  if (!response.IsDouble()) {
    throw YggInterface::YggdrasilRuntimeError("Did not receive a scalar response");
  }
  return response.GetDouble();
}

#else // HAVE_YGGINTERFACE

// Stubs to allow compilation w/ error on use

YggScalar::YggScalar(string const &name, unsigned int npar) :
  ScalarFunction(name, npar) {
  MISSING_YGGINTERFACE_ERROR
}
YggScalar::~YggScalar() {}
string YggScalar::deparse(vector<string> const &) const { return ""; }
double YggScalar::evaluate(vector <double const *> const &) const {
  return 0; }


#endif // HAVE_YGGINTERFACE
