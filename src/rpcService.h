#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H

#include <torch/torch.h>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "runtime.grpc.pb.h"

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;

/**
 * GRPC service implementation for runtime.
 */
class AccumulatorServiceImpl final : public Accumulator::Service {
 public:
  RuntimeServiceImpl()
    : Runtime::Service() {}

//  private:
 public: // temporarily made public for testing.
  Status InitCommGRPC(ServerContext* context,
                      const InitCommGRPCRequest* request,
                      StandardReply* reply) override;
  Status InitCommNCCL(ServerContext* context,
                      const InitCommNCCLMsg* request,
                      InitCommNCCLMsg* reply) override;
  Status ScheduleTraining(ServerContext* context,
                          const ScheduleTrainingRequest* request,
                          StandardReply* reply) override;
  Status Poke(ServerContext* context, const Empty* request,
              StandardReply* reply) override;
  Status Shutdown(ServerContext* context, const Empty* request,
                  StandardReply* reply) override;
  Status P2PCommunication(ServerContext* context,
                          const P2PCommunicationRequest* request,
                          StandardReply* reply) override;

  void setGrpcServer(grpc::Server* serverPtr);
  
 private:
  grpc::Server* grpcServerPtr;
};

/**
 * GRPC client for runtime service.
 * It's particially implemented as most services are used by cluster.py.
 */
class RuntimeClient {
 public:
  RuntimeClient(std::shared_ptr<grpc::ChannelInterface> channel)
    : stub_(Runtime::NewStub(channel)) {}
  
  std::string Poke();
  std::string P2PCommunication(const std::string& taskName,
                               const std::string& tsrData, int tag);

 private:
  std::unique_ptr<Runtime::Stub> stub_;
};


#endif // RPC_SERVICE_H