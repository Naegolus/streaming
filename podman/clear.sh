#!/bin/bash

podman ps -a | awk '{print $1}' | xargs podman rm
podman images | grep none | awk '{print $3}' | xargs podman rmi
