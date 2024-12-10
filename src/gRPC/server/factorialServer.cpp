#include <iostream>
#include <string>
#include <sstream>
#include <grpcpp/grpcpp.h>
#include "factorial.grpc.pb.h"
#include "factorialServer.hpp"

int factorialCalc (int n) {

    int returnVal = 1;
    if (n < 0) {
        std::cout << "Error! Factorial of a negative number doesn't exist." << std::endl;;
        returnVal = 0;
    } else {
        for(int i = 1; i <= n; ++i) {
            returnVal *= i;
        }  
    }
    return returnVal;
}

FactorialServer::FactorialServer(std::string address)
    : address(address) {}

grpc::Status FactorialServer::sendRequest(grpc::ServerContext* context, const factorial::FactorialRequest* request, factorial::FactorialReply* reply) {

    grpc::Status returnVal;
    std::string a = request->a();
    int input;

    // Convert the string to an integer
    std::stringstream ss(a);
    if (ss >> input && ss.eof()) {

        reply->set_result(factorialCalc(input));
        returnVal = grpc::Status::OK;


    } else {

        std::cout << "Invalid integer input.\n";
        returnVal = grpc::Status(grpc::StatusCode::INTERNAL, "The input string can not be converted into a number.");

    }
    
    return returnVal;

}

void FactorialServer::run() {

    grpc::ServerBuilder builder;

    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << address << std::endl;

    server->Wait();    

}