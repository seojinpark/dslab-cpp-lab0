#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "accumulator.grpc.pb.h"

using grpc::ServerContext;
using grpc::Status;

/**
 * GRPC client for Accumulator service.
 */
class RpcClient {
 public:
  RpcClient(std::shared_ptr<grpc::ChannelInterface> channel)
    : stub_(Accumulator::NewStub(channel)) {}
  
  std::string AddWordCount(std::string text);
  int GetWordCount();
  std::string ResetCounter();
  std::string Shutdown();

 private:
  std::unique_ptr<Accumulator::Stub> stub_;
};

#endif // RPC_CLIENT_H