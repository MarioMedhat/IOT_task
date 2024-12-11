#ifndef HTTPClient_HPP
#define HTTPClient_HPP

#include <string>

class HTTPClient {
    public:
        // Function to send an HTTP request
        static std::string sendRequest(const std::string& url, const std::string& method, const std::string& body = "");

    private:

        static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData);
};

#endif // HTTPClient_HPP
