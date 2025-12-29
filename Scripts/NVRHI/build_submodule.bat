::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This source is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : build_submodule.bat
::   
::   Content : NVRHI submodule builder
::
::=================================================

@echo off

pushd "%~dp0\..\..\External\NVRHI"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DNVRHI_WITH_DX12=ON -DNVRHI_WITH_DX11=ON -DNVRHI_WITH_VULKAN=ON -DNVRHI_BUILD_SHARED=OFF
cmake --build build --config Release

popd