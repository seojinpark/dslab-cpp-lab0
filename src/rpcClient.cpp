#include "rpcClient.h"
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "accumulator.grpc.pb.h"

int
RpcClient::AddWordCount(std::string text) {

  AddWordCountRequest request;
  request.set_text(text);

  grpc::ClientContext context;
  AddWordCountReply reply;
  Status status = stub_->AddWordCount(&context, request, &reply);
  if (status.ok()) {
    return reply.word_count();
  } else {
    std::cerr << "Failed to invoke AddWordCount. code: " << status.error_code()
        << " msg: " << status.error_message().c_str() << std::endl;
    return -1;
  }
  // TODO: implement
}

int
RpcClient::GetAllWordCount() {
  Empty request;
  
  grpc::ClientContext context;
  GetAllWordCountReply reply;
  Status status = stub_->GetAllWordCount(&context, request, &reply);
  if (status.ok()) {
    return reply.cummulative_count();
  } else {
    std::cerr << "Failed to invoke AddWordCount. code: " << status.error_code()
        << " msg: " << status.error_message().c_str() << std::endl;
  }

  // TODO: implement
  return 0;
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
