#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "accumulator.grpc.pb.h"

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;

/**
 * GRPC service implementation for runtime.
 */
class AccumulatorServiceImpl final : public Accumulator::Service {
 public:
  AccumulatorServiceImpl()
    : Accumulator::Service() {}

  void setGrpcServer(grpc::Server* serverPtr);
 
 private:
  Status AddWordCount(ServerContext* context,
                      const AddWordCountRequest* request,
                      StandardReply* reply) override;
  Status GetWordCount(ServerContext* context,
                      const Empty* request,
                      GetWordCountReply* reply) override;
  Status ResetCounter(ServerContext* context, const Empty* request,
              StandardReply* reply) override;
  Status Shutdown(ServerContext* context, const Empty* request,
                  StandardReply* reply) override;

  // Pointer to grpc server instance. Used for shutdown.
  grpc::Server* grpcServerPtr;
};

#endif // RPC_SERVICE_H