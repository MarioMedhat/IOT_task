#include <grpcpp/grpcpp.h>
#include "factorial.grpc.pb.h"

class FactorialServer final : public factorial::FactorialService::Service {

    public:
        FactorialServer(std::string address);
        void run();
        grpc::Status sendRequest(grpc::ServerContext* context, const factorial::FactorialRequest* request, factorial::FactorialReply* reply) override;

    private:
        std::string address;
};