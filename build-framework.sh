#!/bin/bash

# Script to prepare a Board Support Package for the CMU family of ARM Cortex-M3 Arduino boards.
#
# Created on 2019-0531 by Bob Iannucci

# Usage:
#
#   build-bsp [version] [source-dir] [package-dir]
#
#   where
#     [version] is in the form x.x.x
#     [source-dir] is the directory containing the BSP
#     [target-dir] is the directory into which the zipped version of the package is to be loaded
#
# If successful, this script will display the SHA-256 hash of the new package as well as its size.
# These need to be edited into package_cmu_index.json
#
# Once that is done, commit the changes to the [target-dir] and push.

SOURCE_DIR="frameworks/commonsense/"
TARGET_FILENAME="framework-commonsense-$1.tar.bz2"

pushd $SOURCE_DIR
find . -type f -name ".DS_Store" -delete
find . -type f -name "._*" -delete
popd
echo "Creating tarball..."
COPYFILE_DISABLE=1 tar -jcf $TARGET_FILENAME $SOURCE_DIR
echo "Computing SHA1 digest..."
openssl dgst -sha1 $TARGET_FILENAME
ls -al $TARGET_FILENAME
