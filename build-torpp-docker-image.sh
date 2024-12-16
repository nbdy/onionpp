#!/bin/bash

IMAGE_NAME="torpp"
IMAGE_VERSION="latest"
DOCKERFILE_PATH="."
EXPORT_PATH="./torpp-image.tar"

while [[ $# -gt 0 ]]; do
  case $1 in
    -i|--image)
      IMAGE_NAME="$2"
      shift 2
      ;;
    -v|--version)
      IMAGE_VERSION="$2"
      shift 2
      ;;
    -f|--dockerfile)
      DOCKERFILE_PATH="$2"
      shift 2
      ;;
    -o|--output)
      EXPORT_PATH="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      echo "Usage: $0 [-i|--image IMAGE_NAME] [-v|--version IMAGE_VERSION] [-f|--dockerfile DOCKERFILE_PATH]"
      exit 1
      ;;
  esac
done

FULL_IMAGE_NAME="${IMAGE_NAME}:${IMAGE_VERSION}"

if docker images --format '{{.Repository}}:{{.Tag}}' | grep -wq "$FULL_IMAGE_NAME"; then
  echo "Docker image '$FULL_IMAGE_NAME' already exists."
else
  echo "Docker image '$FULL_IMAGE_NAME' does not exist. Building it now..."
  docker build -t "$FULL_IMAGE_NAME" "$DOCKERFILE_PATH"

  if [ $? -ne 0 ]; then
    echo "Failed to build Docker image '$FULL_IMAGE_NAME'. Check the Docker logs for more information."
    exit 1
  fi
  echo "Docker image '$FULL_IMAGE_NAME' built successfully."
fi

echo "Exporting Docker image '$FULL_IMAGE_NAME' to '$EXPORT_PATH'..."
docker save -o "$EXPORT_PATH" "$FULL_IMAGE_NAME"

if [ $? -eq 0 ]; then
  echo "Docker image '$FULL_IMAGE_NAME' exported successfully to '$EXPORT_PATH'."
else
  echo "Failed to export Docker image '$FULL_IMAGE_NAME'."
  exit 1
fi
