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

std::string latestReading;
std::string latestTimestamp;
int minReading = 0;
int maxReading = 0;

class SensorController {
private:

    std::mutex mutex;

public:

    void getAllSensorReadings (const Rest::Request &request, Http::ResponseWriter response) {

        auto sensor = request.param(":sensor").as<std::string>();
        auto jsonDataPtr = std::make_shared<nlohmann::json>();

        {
            std::lock_guard<std::mutex> lock(mutex);
            readJsonFile(("../Server/" + sensor + ".json"), jsonDataPtr);
        }

        response.send(Http::Code::Created, jsonDataPtr->dump());
    }

    void addNewSensorReading (const Rest::Request &request, Http::ResponseWriter response) {

        auto body = nlohmann::json::parse(request.body());
        auto sensor = request.param(":sensor").as<std::string>();
        nlohmann::json JsonData;

        {
            std::lock_guard<std::mutex> lock(mutex);

            latestTimestamp = body["timestamp"];
            latestReading = body["reading"];

            JsonData["timestamp"] = latestReading;
            JsonData["reading"] = latestTimestamp;

            addJsonNewData("../Server/" + sensor + ".json", JsonData);
            
        }

        response.send(Http::Code::Created, JsonData.dump());

    }

    void resetSensorReadings (const Rest::Request &request, Http::ResponseWriter response) {

        auto sensor = request.param(":sensor").as<std::string>();

        {

            std::lock_guard<std::mutex> lock(mutex);
            resetJsonFile("../Server/" + sensor + ".json");
        
        }

    }

};

void startServer() {
    Http::Endpoint endpoint(Address(Ipv4::any(), Port(8082)));
    auto opts = Http::Endpoint::options().threads(1);
    endpoint.init(opts);

    Rest::Router router;
    SensorController controller;

    Rest::Routes::Post(router, "/sensors/:sensor", Rest::Routes::bind(&SensorController::addNewSensorReading, &controller));
    Rest::Routes::Get(router, "/sensors/:sensor", Rest::Routes::bind(&SensorController::getAllSensorReadings, &controller));
    Rest::Routes::Delete(router, "/sensors/:sensor", Rest::Routes::bind(&SensorController::resetSensorReadings, &controller));

    endpoint.setHandler(router.handler());
    std::cout << "Server running on port 8082..." << std::endl;
    endpoint.serve();
}
