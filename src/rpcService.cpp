#include "rpcService.h"
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "accumulator.grpc.pb.h"

#define UNUSED(expr) (void)(expr)

using grpc::Server;
using grpc::ServerContext;
using grpc::Status;

void
AccumulatorServiceImpl::setGrpcServer(grpc::Server* serverPtr) {
  grpcServerPtr = serverPtr;
}


//////////////////////////////////////////////////////////////////
// RPC service implementations below
//////////////////////////////////////////////////////////////////

Status
AccumulatorServiceImpl::AddWordCount(ServerContext* context,
    const AddWordCountRequest* request,
    StandardReply* reply) {
  UNUSED(context);
  // TODO: implement
  return Status::OK;
}

Status
AccumulatorServiceImpl::GetWordCount(ServerContext* context,
    const Empty* request,
    GetWordCountReply* reply) {
  UNUSED(context);
  UNUSED(request);
  // TODO: implement
  return Status::OK;
}

Status
AccumulatorServiceImpl::ResetCounter(ServerContext* context,
    const Empty* request, StandardReply* reply) {
  UNUSED(context);
  UNUSED(request);

  // TODO: implement
}

Status
AccumulatorServiceImpl::Shutdown(ServerContext* context, const Empty* request,
    StandardReply* reply) {
  UNUSED(context);
  UNUSED(request);

  std::cout << "Shutdown requested.";
  std::string replyMsg("Shutdown invoked.");
  reply->set_message(replyMsg);
  grpcServerPtr->Shutdown();
  return Status::OK;
}
