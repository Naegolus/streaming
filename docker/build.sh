#!/bin/bash

proj="dspc-bdpt"

cd $(dirname $0)
dirDocker="$(pwd)"

docker build -t "$proj" --build-arg user="$USER" --build-arg uid="$(id -u)" --build-arg gid="$(id -g)" .
