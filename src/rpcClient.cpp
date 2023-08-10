#include "rpcClient.h"
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "accumulator.grpc.pb.h"

int
RpcClient::AddWordCount(std::string text) {
  AddWordCountRequest request;
  request.set_text(text);
  // TODO (Milestone1): implement rest.
}

int
RpcClient::GetAllWordCount() {
  // TODO (Milestone1): implement
  return -1;
}

std::string
RpcClient::ResetCounter() {
  // TODO: implement.
  return "Not implemented";
}

std::string
RpcClient::Shutdown() {
  Empty request;
  grpc::ClientContext context;
  StandardReply reply;
  Status status = stub_->Shutdown(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  } else {
    std::cerr << "Failed to request shutdown. code: "<< status.error_code()
              <<  " msg: %s." << status.error_message().c_str();
    return "Failed to request shutdown.";
  }
}
