#include "rpcClient.h"
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "runtime.grpc.pb.h"

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

std::string
RpcClient::ResetCounter() {
  Empty request;
  grpc::ClientContext context;
  StandardReply reply;
  Status status = stub_->ResetCounter(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  } else {
    std::cerr << "Failed to request reset. code: "<< status.error_code()
              <<  " msg: %s." << status.error_message().c_str();
    return "Failed to request reset.";
  }
}

std::string
RpcClient::P2PCommunication(const std::string& taskName,
                            const std::string& tsrData, int tag) {
  P2PCommunicationRequest request;
  request.set_task_name(taskName);
  request.set_tensor_data(tsrData);
  request.set_tag(tag);

  grpc::ClientContext context;
  StandardReply reply;
  Status status = stub_->P2PCommunication(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  } else {
    DP_LOG(ERROR, "Failed to invoke P2PCommunication. code: %d, msg: %s.",
          status.error_code(), status.error_message().c_str());
    return "Failed to invoke P2PCommunication.";
  }
}