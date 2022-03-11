#!/bin/bash

proj="twitch"

cd $(dirname $0)
dirPodman="$(pwd)"

img="$proj"
if [ ! -z "$1" ]; then
	img="$1"
fi

shellNameDefault="$(basename $SHELL)"
dirProj="$(basename $(echo $dirPodman | rev | cut -d "/" -f 2 | rev))"

eval $(ssh-agent -s)

mappings="
-v $dirPodman/..:$HOME/$dirProj
-v $HOME/.${shellNameDefault}rc:$HOME/.${shellNameDefault}rc
-v $HOME/.${shellNameDefault}_history:$HOME/.${shellNameDefault}_history
-v $HOME/.gitconfig:$HOME/.gitconfig
"

if [ ! -z "$SSH_AUTH_SOCK" ]; then
	mappings="$mappings
	-v $(dirname $SSH_AUTH_SOCK):$(dirname $SSH_AUTH_SOCK) 
	-e SSH_AUTH_SOCK=$SSH_AUTH_SOCK 
	"
fi

echo "Running image $img"

podman run --rm $mappings --user $(id -u):$(id -g) --dns=8.8.8.8 -it $img entry.sh \
	"cd $HOME/$dirProj" \
	"$SHELL"

