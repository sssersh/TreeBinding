#!/bin/bash

cd $1
conan install .. -s build_type=Debug --build=missing
