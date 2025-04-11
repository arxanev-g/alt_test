#include <string>
#include "../include/Loader.h"
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/URI.h>
#include <Poco/JSON/Object.h>

class Loader : public BaseLoader {
private:
    std::string branch_;
    std::string api_endpoint_;

public:
    Loader(const std::string& branch, const std::string& endpoint)
        : branch_(branch), api_endpoint_(endpoint) {
    }

    Map Load() {
        Poco::URI uri(api_endpoint_ + branch_);
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());
        std::string path = uri.getPathAndQuery();
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
        session.sendRequest(request);
        Poco::Net::HTTPResponse respons;
        auto& body = session.receiveResponse(respons);
        if (respons.getStatus() != 200) {
            throw APIError(respons.getStatus(), "ERROR");
        }
        Poco::JSON::Parser parser;
        auto reply = parser.parse(body);

        auto root = reply.extract<Poco::JSON::Object::Ptr>();
        auto packages = root->getArray("packages");

        Map result;
        for (size_t iter = 0; iter < packages->size(); ++iter) {
            auto pkg = packages->getObject(iter);
            std::string arch = pkg->getValue<std::string>("arch");
            std::string name = pkg->getValue<std::string>("name");
            result[arch][name] = { branch_,
                pkg->getValue<std::string>("version"),
                                  pkg->getValue<std::string>("release")};
        }
 
        return result;
    }
};

std::unique_ptr<BaseLoader> CreateLoader(const std::string& branch,
                                              const std::string& endpoint) {
    return std::make_unique<Loader>(branch, endpoint);
}
