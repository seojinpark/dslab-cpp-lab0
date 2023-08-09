#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <getopt.h>
#include "rpcService.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "runtime.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;



char* myAddr;           // includes port number.
std::unique_ptr<RuntimeServiceImpl> grpcService;
std::unique_ptr<grpc::Server> grpcServer;


void initGrpcServer() {
  std::string server_address(myAddr);
  grpcService = std::make_unique<AccumulatorServiceImpl>();
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(grpcService.get());
  grpcServer = builder.BuildAndStart(); //.release();
  grpcService->setGrpcServer(grpcServer.get());
  std::cout << "Server listening on " << server_address << std::endl;
}

void parse_args(int argc, char** argv) {
  static struct option long_options[] = {
      {"myAddr", required_argument, NULL, 'm'},
      {NULL, 0, NULL, 0}
  };

  // loop over all of the options
  char ch;
  while ((ch = getopt_long(argc, argv, "t:a:", long_options, NULL)) != -1) {
    switch (ch) {
      case 'm':
        myAddr = optarg;
        break;


      default:
        printf("?? getopt returned character code 0%o ??\n", ch);
    }
  }
}

int main(int argc, char** argv) {
  shutdownRequested = false;
  parse_args(argc, argv);
  
  std::cout << "myAddr: " << myAddr << std::endl;
  initGrpcServer();
  // grpcServer->Shutdown();
  // std::cout << "grpc shutdown." << std::endl;
  grpcServer->Wait();
  return 0;
}