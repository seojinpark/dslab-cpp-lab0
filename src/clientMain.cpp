#include <iostream>
#include <memory>
#include <string>
#include <getopt.h>
#include "rpcClient.h"

char* ipAndPort;           // includes port number.
char* inputText;
bool resetCounter = false;
bool shutdownServer = false;

void parse_args(int argc, char** argv) {
  static struct option long_options[] = {
      {"dest", required_argument, NULL, 'i'},
      {"reset", no_argument, NULL, 'r'},
      {"shutdown", no_argument, NULL, 's'},
      {"text", required_argument, NULL, 't'},
      {NULL, 0, NULL, 0}
  };

  // loop over all of the options
  signed char ch;
  while ((ch = getopt_long(argc, argv, "t:a:", long_options, NULL)) != -1) {
    switch (ch) {
      case 'i':
        ipAndPort = optarg;
        break;
      case 'r':
        resetCounter = true;
        break;
      case 's':
        shutdownServer = true;
        break;
      case 't':
        inputText = optarg;
        break;
      default:
        printf("?? getopt returned character code 0%o ??\n", ch);
    }
  }
}

int main(int argc, char** argv) {
  parse_args(argc, argv);

  auto channel = grpc::CreateChannel(ipAndPort, grpc::InsecureChannelCredentials());
  RpcClient client(channel);

  if (resetCounter) {
    // TODO: implement
  } else if (shutdownServer) {
    client.Shutdown();
    std::cout << "Shutdown requested." << std::endl;
  } else {
    // TODO (Milestone3): modify below to use just one RPC.
    int wc = client.AddWordCount(inputText);
    int wcSum = client.GetAllWordCount();
    std::cout << "Word count: " << wc << std::endl
              << "Sum of all word counts: " << wcSum << std::endl;
  }

  return 0;
}
