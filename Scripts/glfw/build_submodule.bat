::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This file is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : build_submodule.bat
::   
::   Content : GLFW submodule builder
::
::=================================================

@echo off

pushd "%~dp0\..\..\External\glfw"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config MinSizeRel

popd