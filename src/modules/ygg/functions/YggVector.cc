#include "YggVector.h"

using namespace jags::ygg;

#ifdef HAVE_YGGINTERFACE

YggVectorLoader::YggVectorLoader(std::string const &name0,
                                 unsigned int npar0) :
  YggLoader(name0, npar0), length_res(1), length_par() {
  if (doc.IsObject()) {
    if (doc.HasMember("length_res"))
      length_res = doc["length_res"].GetUint();
    if (doc.HasMember("length_par")) {
      length_par = this->_value2vector(doc["length_par"]);
      if (length_par.size() != this->npar) {
        if (doc.HasMember("par"))
          throw YggInterface::YggdrasilRuntimeError("Length of length_par (" + std::to_string(length_par.size()) + ") does not match provided npar (" + std::to_string(this->npar) + ")");
        this->npar = length_par.size();
      }
    }
  }
}

YggVector::YggVector(const YggVectorLoader& loader) :
  VectorFunction(loader.name, loader.npar), comm(nullptr),
  exp_length(loader.length_res) {
  if (!loader.address.empty()) {
    comm = new YggInterface::communicator::FunctionComm(
      loader.name, loader.address,
      SEND, COMM_FLAG_INTERFACE);
  } else {
    comm = new YggInterface::YggFunction(loader.name);
  }
  if (!(comm && comm->valid()))
    throw YggInterface::YggdrasilRuntimeError("Invalid YggInterface comm \"" + loader.name + "\" (address = \"" + loader.address + "\")");
  // TODO: Add expected length to the schema
  if (loader.npar == 0) {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"1darray\","
                    "    \"subtype\": \"float\""
                    "  }"
                    "}", false, SEND);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"array\","
                    "  \"items\": {"
                    "    \"type\": \"1darray\","
                    "    \"subtype\": \"float\""
                    "  },"
                    "  \"minItems\": " + std::to_string(loader.npar) + "," +
                    "  \"maxItems\": " + std::to_string(loader.npar) +
                    "}", false, SEND);
  }
  if (exp_length == 0) {
    comm->addSchema("{"
                    "  \"type\": \"1darray\","
                    "  \"subtype\": \"float\""
                    "}", false, RECV);
  } else {
    comm->addSchema("{"
                    "  \"type\": \"1darray\","
                    "  \"subtype\": \"float\","
                    "  \"length\": " + std::to_string(exp_length) +
                    "}", false, RECV);
  }
}

YggVector::YggVector(std::string const &name, unsigned int npar) :
  YggVector(YggVectorLoader(name, npar)) {}

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
  if (comm->send(request) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error sending request to the model");
  }
  if (comm->recv(response) < 0) {
    throw YggInterface::YggdrasilRuntimeError("Error receiving response from the model");
  }
  if (!response.Is1DArray()) {
    throw YggInterface::YggdrasilRuntimeError("Did not receive a vector response");
  }
  response.Get1DArray(value, length(lengths, args));
}

unsigned int 
YggVector::length(std::vector <unsigned int> const &lengths,
                  std::vector <double const *> const &) const {
  // Assumes that the function returns a vector of the same size
  if (exp_length == 0) {
    auto max_it = std::max_element(lengths.begin(), lengths.end());
    return *max_it;
  }
  return exp_length;
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
