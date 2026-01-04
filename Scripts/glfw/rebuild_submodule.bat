::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This file is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : rebuild_submodule.bat
::   
::   Content : force build aka rebuild glfw submodule
::
::=================================================

@echo off

pushd "%~dp0\..\..\External\glfw"

rmdir /q /s build

popd

build_submodule.bat