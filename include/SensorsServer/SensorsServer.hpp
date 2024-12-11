#ifndef SENSOR_CONTROLLER_HPP
#define SENSOR_CONTROLLER_HPP

#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <mutex>
#include <string>
#include "JsonDataRW.hpp"

class SensorsServer {
    private:
        void getAllSensorReadings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
        void addNewSensorReading(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
        void resetSensorReadings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

        std::mutex mutex;
        Pistache::Http::Endpoint endpoint;

    public:
        SensorsServer(Pistache::Address address);
        void startServer();
};

#endif // SENSOR_CONTROLLER_HPP
