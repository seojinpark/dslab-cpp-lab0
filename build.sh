#!/bin/bash

# Arguments
while getopts ":j:" opt; do
  case $opt in
    j) JOBS="$OPTARG"
    ;;
    \?) echo "Invalid option -$OPTARG" >&2
    exit 1
    ;;
  esac

  case $OPTARG in
    -*) echo "Option $opt needs a valid argument"
    exit 1
    ;;
  esac
done

# Environment variables
set -x

# GRPC build for python
# python -m grpc_tools.protoc -Icsrc/protos --python_out=. --grpc_python_out=. csrc/protos/runtime.proto

# CPP runtime build.
mkdir -p build
pushd build/
cmake ..

if [[ -z "$JOBS" ]]; then
  echo "Jobs number is not specified. Running with make -j" 
  make -j
else
  echo "Setting jobs number as $JOBS..."
  make -j$JOBS
fi

popd
