#include "YggScalar.h"

using std::vector;
using std::string;

namespace jags {
namespace ygg {

YggScalar::YggScalar(string const &name, unsigned int npar) :
  ScalarFunction(name, npar), comm(nullptr) {
  comm = new YggInterface::YggRpcClient(name);
  if (npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"}"
                    "}", false, YggInterface::SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {\"type\": \"number\"},"
                    "  \"minItems\": " + to_string(npar) + "," +
                    "  \"maxItems\": " + to_string(npar) +
                    "}", false, YggInterface::SEND);
  }
  comm->addSchema("{"
                  "  \"type\": \"number\""
                  "}", false, YggInterface::RECV);
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
    throw std::exception("Error communicating with the model");
  }
  if (!response.IsDouble()) {
    throw std::exception("Did not receive a scalar response");
  }
  return response.GetDouble();
}
