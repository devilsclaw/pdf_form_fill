#!/bin/bash

if ! [ -d build ] ; then
  mkdir build
fi

if ! [ -d output ] ; then
  mkdir output
fi


cd build

cmake ..

make && ./main ../sample-forms/OoPdfFormExample.pdf ../output/OoPdfFormExample.pdf

cd -
