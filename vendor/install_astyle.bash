#!/usr/bin/env bash

# This script installs the Artistic Style code formatter into the
# user's ~/.local directory.

set -o errexit
set -o nounset
set -o xtrace

readonly PREFIX=$HOME/.local
readonly ASTYLE_VERSION=2.02.1
readonly ASTYLE_TAR_FILE=astyle_${ASTYLE_VERSION}_linux.tar.gz

cd /tmp
wget "http://sourceforge.net/projects/astyle/files/astyle/"\
"astyle%20$ASTYLE_VERSION/$ASTYLE_TAR_FILE"
tar -xf "$ASTYLE_TAR_FILE"
rm "$ASTYLE_TAR_FILE"
pushd astyle/build/gcc

# Replace janked install command. The original version fails in our
# development environment.
sed \
	--in-place \
	's/INSTALL=install -o $(USER) -g $(USER)/INSTALL=install/' \
	Makefile

make
make prefix="$PREFIX" install
popd
rm -r astyle
