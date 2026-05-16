#! /bin/bash

# just use gnu make for this in usual build dir
cmake -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build
