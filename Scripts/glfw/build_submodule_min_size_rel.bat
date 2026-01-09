::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This file is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : build_submodule_min_size_rel.bat
::   
::   Content : GLFW submodule builder.
::	Builds without docs, examples and test.
::	Uses MinSizeRel config
::
::=================================================

@echo off

pushd "%~dp0\..\..\External\glfw"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
cmake --build build --config MinSizeRel

popd