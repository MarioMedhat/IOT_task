#include <iostream>
#include <cstring>
#include "Sensor.hpp"
#include "JsonDataRW.hpp"
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>

#include <nlohmann/json.hpp>

std::string getCurrentTimestamp2() {
    // Get current time
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

Temp_Sensor::Temp_Sensor (std::string topic) :
    topic (std::move(topic)) {};

void Temp_Sensor::update_sensor_reading()
{
        static int count = 0;
        count ++;
        timestamp = getCurrentTimestamp2();
        reading = count;
}

nlohmann::json Temp_Sensor::get_sensor_reading()
{
    nlohmann::json JsonData;
    JsonData["timestamp"] = timestamp;
    JsonData["reading"] = reading;

    return JsonData;
}

std::string Temp_Sensor::get_sensor_topic()
{
    return topic;
}

Hum_Sensor::Hum_Sensor (std::string topic) :
    topic (std::move(topic)) {};

void Hum_Sensor::update_sensor_reading()
{
        static int count = 0;
        count ++;
        timestamp = getCurrentTimestamp2();
        reading = (count + 100);
}

nlohmann::json Hum_Sensor::get_sensor_reading()
{
    nlohmann::json JsonData;
    JsonData["timestamp"] = timestamp;
    JsonData["reading"] = reading;

    return JsonData;
}

std::string Hum_Sensor::get_sensor_topic()
{
    return topic;
}