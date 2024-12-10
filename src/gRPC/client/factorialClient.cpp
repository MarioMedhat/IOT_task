#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "factorial.grpc.pb.h"
#include "factorialClient.hpp"

FactorialClient::FactorialClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(factorial::FactorialService::NewStub(channel)) {}


int FactorialClient::sendRequest(std::string a) {
    factorial::FactorialRequest request;

    request.set_a(a);
    factorial::FactorialReply reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->sendRequest(&context, request, &reply);

    if(status.ok()){
        return reply.result();
    } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        return -1;
    }
}