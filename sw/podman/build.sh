#!/bin/bash

proj="twitch"

cd $(dirname $0)
dirPodman="$(pwd)"

docker build -t "$proj" --build-arg user="$USER" --build-arg uid="$(id -u)" --build-arg gid="$(id -g)" .
