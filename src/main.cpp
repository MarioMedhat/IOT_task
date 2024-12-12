#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>
#include <cstring>

#include "mqtt/async_client.h"

#include "MQTT_Client.hpp"
#include "JsonDataRW.hpp"
#include "Sensor.hpp"
#include "factorialServer.hpp"
#include "factorialClient.hpp"
#include "SensorsServer.hpp"

// Définir l'adresse et le port pour le serveur HTTP
Pistache::Address address(Pistache::Ipv4::any(), Pistache::Port(8080));

// Créer une instance de SensorsServer
SensorsServer controller(address);

void FactorialClientrun() {

    std::string address("0.0.0.0:5000");
    FactorialClient client(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    int response;

    while (true) {

        std::string input;
        std::cout << "Enter a number to compute it's factorial:     ";
        std::getline(std::cin, input); // Read the entire line as a string

        response = client.sendRequest(input);
        if (response > 0) {

            std::cout << "Answer received = " << response << std::endl;

        }

    }

}

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID_P("publisher");
const std::string CLIENT_ID_S("subscriber");
const std::string TOPIC("test/topic");
const int KEEP_ALLIVE_INTERVALL = 20;

const int QOS = 1;
const int TIMEOUT = 10000;

int main(int argc, char** argv) {

    std::thread serverThread([&controller]() {controller.startServer();});
    // std::thread clientThread(runClient);

    SensorService sensor (SERVER_ADDRESS, "Sensors", 20, 1);
    LoggerService logger(SERVER_ADDRESS, "logger", 20, 1);
    //std::thread th1(sensors_main);
    //std::thread th2(pub_main);
    //std::thread th3(sub_main);
    std::thread th1([&sensor]() {sensor.sensor_update();});
    std::thread th2([&sensor]() {sensor.sensor_publish();});
    std::thread th3([&logger]() {logger.read_sensors();});

    th1.join();
    th2.join();
    th3.join();
    serverThread.join();
    // clientThread.join();
    return 0;
}
