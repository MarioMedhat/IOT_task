#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <mutex>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <ctime>
#include <fstream>
#include "JsonDataRW.hpp"

using namespace Pistache;

// Function to handle the response data for the client
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    userData->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to send an HTTP request from the client
std::string sendRequest(const std::string& url, const std::string& method, const std::string& body = "") {
    std::cout << "youppi" << std::endl;
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        if (method == "POST" || method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
            if (method == "PUT") {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            }
        } else if (method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return response;
}

void runClient() {
    std::string baseUrl = "http://localhost:8082/sensors";

    int count = 0;

    std::string getResponse = sendRequest(baseUrl + "/temperature", "GET");
    std::cout << "GET All Books Response: " << getResponse << std::endl;

    std::string deleteResponse = sendRequest(baseUrl + "/temperature", "DELETE");

}