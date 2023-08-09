#include "rpcService.h"
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "runtime.grpc.pb.h"

using json = nlohmann::json;
using grpc::Server;
using grpc::ServerContext;
using grpc::Status;

void
AccumulatorServiceImpl::setGrpcServer(grpc::Server* serverPtr)
{
  grpcServerPtr = serverPtr;
}

Status
AccumulatorServiceImpl::InitCommGRPC(ServerContext* context,
    const InitCommGRPCRequest* request,
    StandardReply* reply)
{
  UNUSED(context);
  DP_LOG(DEBUG, "Received InitCommGRPC().");

  json rankToIpMapJson = json::parse(request->rank_to_ip_map_in_json());
  
  rtctx->rankToIpAndPort.resize(rankToIpMapJson.size());
  for (auto& el : rankToIpMapJson.items()) {
    int rank = atoi(el.key().c_str());
    std::string ipAndPort = el.value().get<std::string>();
    rtctx->rankToIpAndPort[rank] = ipAndPort;
    DP_LOG(DEBUG, "Rank %d 's address: %s", rank, ipAndPort.c_str());
  }
  rtctx->grpcCommReady = true;

  std::string replyMsg("InitCommGRPC invoked.");
  reply->set_message(replyMsg);
  return Status::OK;
}

Status
RuntimeServiceImpl::Poke(ServerContext* context, const Empty* request,
    StandardReply* reply)
{
  UNUSED(context);
  UNUSED(request);

  DP_LOG(DEBUG, "poked.");
  std::string replyMsg("Poke invoked.");
  reply->set_message(replyMsg);
  return Status::OK;
}

Status
RuntimeServiceImpl::Shutdown(ServerContext* context, const Empty* request,
    StandardReply* reply)
{
  UNUSED(context);
  UNUSED(request);

  DP_LOG(NOTICE, "Shutdown requested.");
  rtctx->shutdownRequested = true;
  std::cout << "shutdownRequested " << rtctx->shutdownRequested.load() << std::endl;
  std::string replyMsg("Shutdown invoked.");
  reply->set_message(replyMsg);
  return Status::OK;
}


////////////////////////////////////////////////////////
// GRPC Client code.
////////////////////////////////////////////////////////

std::string
RuntimeClient::Poke() {
  Empty request;
  grpc::ClientContext context;
  StandardReply reply;
  Status status = stub_->Poke(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  } else {
    DP_LOG(ERROR, "Failed to invoke Poke. code: %d, msg: %s.",
          status.error_code(), status.error_message().c_str());
    return "Failed to invoke Poke.";
  }
}

std::string
RuntimeClient::P2PCommunication(const std::string& taskName,
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