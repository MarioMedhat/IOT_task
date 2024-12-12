#include <iostream>
#include <fstream>
#include "SensorsServer.hpp"

using namespace Pistache;

// Constructeur
SensorsServer::SensorsServer(Address address) : endpoint(address) {}

// Méthode pour récupérer toutes les lectures d'un capteur
void SensorsServer::getAllSensorReadings(const Rest::Request &request, Http::ResponseWriter response) {
    auto sensor = request.param(":sensor").as<std::string>();
    auto jsonDataPtr = std::make_shared<nlohmann::json>();

    {
        std::lock_guard<std::mutex> lock(mutex);
        readJsonFile("../Server/readings.json", jsonDataPtr);
    }

    response.send(Http::Code::Ok, jsonDataPtr->dump());
}

// Méthode pour ajouter une nouvelle lecture pour un capteur
void SensorsServer::addNewSensorReading(const Rest::Request &request, Http::ResponseWriter response) {
    auto body = nlohmann::json::parse(request.body());
    auto sensor = request.param(":sensor").as<std::string>();
    nlohmann::json JsonDataReadings;

    {
        std::lock_guard<std::mutex> lock(mutex);

        // std::cout << "ana giiiiiiiiit " << body.dump() << std::endl;

        JsonDataReadings["timestamp"] = body["timestamp"];
        JsonDataReadings["reading"] = body["reading"];
        JsonDataReadings["id"] = sensor;

        addJsonNewData("../Server/readings.json", JsonDataReadings);

        auto JsonDataStatsPtr = std::make_shared<nlohmann::json>();
        nlohmann::json JsonDataStats;
        bool isFirstTime = isValueInField("../Server/stats.json", "id", sensor, JsonDataStatsPtr);

        if (isFirstTime == false) {
            JsonDataStats["id"] = sensor;
            JsonDataStats["min"] = body["reading"];
            JsonDataStats["max"] = body["reading"];
            JsonDataStats["avg"] = body["reading"];
        } else {
            JsonDataStats = (*JsonDataStatsPtr);
            if (JsonDataStats["min"] > body["reading"]) {
                JsonDataStats["min"] = body["reading"];
            } else if (JsonDataStats["max"] < body["reading"]) {
                JsonDataStats["max"] = body["reading"];
            }

            JsonDataStats["avg"] = ((float)JsonDataStats["avg"] + (float)body["reading"]) / (float)2;

            std::cout << JsonDataStats.dump() << std::endl;
        }
        updateObject("../Server/stats.json", "id", sensor, JsonDataStats);
    }

    response.send(Http::Code::Created, JsonDataReadings.dump());
}

// Méthode pour réinitialiser les lectures d'un capteur
void SensorsServer::resetSensorReadings(const Rest::Request &request, Http::ResponseWriter response) {
    auto sensor = request.param(":sensor").as<std::string>();

    {
        std::lock_guard<std::mutex> lock(mutex);
        resetJsonFile("../Server/" + sensor + ".json");
    }

    response.send(Http::Code::No_Content);
}

// Méthode pour démarrer le serveur
void SensorsServer::startServer() {
    auto opts = Http::Endpoint::options().threads(1);
    endpoint.init(opts);

    Rest::Router router;

    Rest::Routes::Post(router, "/sensors/:sensor", Rest::Routes::bind(&SensorsServer::addNewSensorReading, this));
    Rest::Routes::Get(router, "/sensors/:sensor", Rest::Routes::bind(&SensorsServer::getAllSensorReadings, this));
    Rest::Routes::Delete(router, "/sensors/:sensor", Rest::Routes::bind(&SensorsServer::resetSensorReadings, this));

    endpoint.setHandler(router.handler());
    //std::cout << "Server running on port " << endpoint.address().port() << "..." << std::endl;
    endpoint.serve();
}
