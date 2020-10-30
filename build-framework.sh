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

VERSION=$1
SOURCE_DIR="frameworks/commonsense/"
PACKAGE_FILENAME=$SOURCE_DIR"package.json"
TARGET_FILENAME="framework-commonsense-$VERSION.tar.bz2"

echo "Updating version in package.json"
node update_framework_package_json.js $PACKAGE_FILENAME $VERSION

pushd $SOURCE_DIR
find . -type f -name ".DS_Store" -delete
find . -type f -name "._*" -delete
popd
echo "Creating tarball..."
COPYFILE_DISABLE=1 tar -jcf $TARGET_FILENAME $SOURCE_DIR

echo "Computing SHA1 digest..."
SHA=`openssl dgst -sha1 $TARGET_FILENAME | sed 's/^.* //'`  #https://unix.stackexchange.com/questions/42797/openssl-dgst-sha1-producing-an-extraneous-stdin-prefix-and-trailing-new 
ls -al $TARGET_FILENAME
echo $VERSION
node add_to_manifest $TARGET_FILENAME $VERSION $SHA