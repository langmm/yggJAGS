#include "YggScalar.h"

using std::vector;
using std::string;

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggScalarLoader::YggScalarLoader(std::string const &name,
                                 unsigned int npar0) :
  YggLoader(name, npar0) {}

YggScalar::YggScalar(const YggScalarLoader& loader) :
  ScalarFunction(loader.name, loader.npar), comm(nullptr) {
  if (!loader.address.empty()) {
    comm = new YggInterface::communicator::FunctionComm(
      loader.name, loader.address,
      SEND, COMM_FLAG_INTERFACE);
  } else {
    comm = new YggInterface::YggFunction(loader.name);
  }
  if (!(comm && comm->valid()))
    throw YggInterface::YggdrasilRuntimeError("Invalid YggInterface comm \"" + loader.name + "\" (address = \"" + loader.address + "\")");
  if (loader.npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"}"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"},"
                    "  \"minItems\": " + std::to_string(loader.npar) + "," +
                    "  \"maxItems\": " + std::to_string(loader.npar) +
                    "}", false, SEND);
  }
  comm->addSchema("{"
                  "  \"type\": \"number\""
                  "}", false, RECV);
}

YggScalar::YggScalar(string const &name, unsigned int npar) :
  YggScalar(YggScalarLoader(name, npar)) {}

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
  if (comm->send(request) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error sending request to the model");
  }
  if (comm->recv(response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error receiving response from the model");
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
