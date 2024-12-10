#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class Sensor
{
    public:
        virtual void update_sensor_reading() = 0;
        virtual nlohmann::json get_sensor_reading() = 0;
        virtual std::string get_sensor_topic() = 0;

        std::string timestamp;
        nlohmann::json reading;

};

class Temp_Sensor : public Sensor
{
    public:
        Temp_Sensor (std::string topic);
        void update_sensor_reading() override;
        nlohmann::json get_sensor_reading() override;
        std::string get_sensor_topic() override;
    
    private:
        std::string topic;
};

class Hum_Sensor : public Sensor
{
    public:
        Hum_Sensor (std::string topic);
        void update_sensor_reading() override;
        nlohmann::json get_sensor_reading() override;
        std::string get_sensor_topic() override;
    
    private:
        std::string topic;
};

#endif