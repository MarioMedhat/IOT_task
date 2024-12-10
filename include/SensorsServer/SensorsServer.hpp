#ifndef JSON_SERVER_CLIENT_HPP
#define JSON_SERVER_CLIENT_HPP

#include <string>
#include <nlohmann/json.hpp>

class JsonServerClient {
public:
    explicit JsonServerClient(const std::string& baseUrl);

    nlohmann::json get(const std::string& endpoint) const;
    nlohmann::json post(const std::string& endpoint, const nlohmann::json& payload) const;
    nlohmann::json put(const std::string& endpoint, const nlohmann::json& payload) const;
    nlohmann::json del(const std::string& endpoint) const;

private:
    std::string baseUrl_;
};

#endif // JSON_SERVER_CLIENT_HPP
