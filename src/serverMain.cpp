#include <atomic>
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <getopt.h>
#include "rpcService.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "accumulator.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;



char* myAddr;           // includes port number.
std::unique_ptr<AccumulatorServiceImpl> grpcService;
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
      {"bind", required_argument, NULL, 'i'},
      {NULL, 0, NULL, 0}
  };

  // loop over all of the options
  signed char ch;
  while ((ch = getopt_long(argc, argv, "t:a:", long_options, NULL)) != -1) {
    switch (ch) {
      case 'i':
        myAddr = optarg;
        break;

      default:
        printf("?? getopt returned character code 0%o ??\n", ch);
    }
  }
}

int main(int argc, char** argv) {
  parse_args(argc, argv);
  
  initGrpcServer();
  grpcServer->Wait();
  
  shutdown_thread->join();
  return 0;
}
