#!/bin/bash
echo -ne "\033[0;32m"
echo 'Updating bazel dependencies.'
echo -ne "\033[0m"
set -e

if [ "$(uname -s)" == "Linux" ]; then
  BAZEL_DEPS_URL=https://github.com/johnynek/bazel-deps/releases/download/v0.1-13/bazel-deps-linux
  BAZEL_DEPS_SHA256=e3993d5683884081af3076ce9d3dafdf7c6ba591bf12d7bb82cb5afd6a954681
elif [ "$(uname -s)" == "Darwin" ]; then
  BAZEL_DEPS_URL=https://github.com/johnynek/bazel-deps/releases/download/v0.1-13/bazel-deps-macos
  BAZEL_DEPS_SHA256=a9189380ec1b2278234a24d29817fdb095e435ca66bd4aef80715a47110425cd
else
  echo "Your platform '$(uname -s)' is unsupported, sorry"
  exit 1
fi


# This is some bash snippet designed to find the location of the script.
# we operate under the presumption this script is checked into the repo being operated on
# so we goto the script location, then use git to find the repo root.
SCRIPT_LOCATION="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_LOCATION

REPO_ROOT=$(git rev-parse --show-toplevel)

BAZEL_DEPS_DIR="$HOME/.bazel-deps-cache"
BAZEL_DEPS_PATH="${BAZEL_DEPS_DIR}/v0.1-13"

if [ ! -f ${BAZEL_DEPS_PATH} ]; then
  ( # Opens a subshell
    set -e
    echo "Fetching bazel deps."
    curl -L -o /tmp/bazel-deps-bin $BAZEL_DEPS_URL

    GENERATED_SHA_256=$(shasum -a 256 /tmp/bazel-deps-bin | awk '{print $1}')

    if [ "$GENERATED_SHA_256" != "$BAZEL_DEPS_SHA256" ]; then
      echo "Sha 256 does not match, expected: $BAZEL_DEPS_SHA256"
      echo "But found $GENERATED_SHA_256"
      echo "You may need to update the sha in this script, or the download was corrupted."
      exit 1
    fi

    chmod +x /tmp/bazel-deps-bin
    mkdir -p ${BAZEL_DEPS_DIR}
    mv /tmp/bazel-deps-bin ${BAZEL_DEPS_PATH}
  )
fi

cd $REPO_ROOT
set +e
$BAZEL_DEPS_PATH generate -r $REPO_ROOT -s 3rdparty/workspace.bzl -d dependencies.yaml  --target-file 3rdparty/target_file.bzl --disable-3rdparty-in-repo
RET_CODE=$?
set -e

if [ $RET_CODE == 0 ]; then
  echo "Success, going to format files"
else
  echo "Failure, checking out 3rdparty/jvm"
  cd $REPO_ROOT
  git checkout 3rdparty/jvm 3rdparty/workspace.bzl
  exit $RET_CODE
fi

$BAZEL_DEPS_PATH format-deps -d $REPO_ROOT/dependencies.yaml -o
