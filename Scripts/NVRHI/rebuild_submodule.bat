::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This source is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : rebuild_submodule.bat
::   
::   Content : force build aka rebuild NVRHI submodule
::
::=================================================

pushd "%~dp0\..\..\External\NVRHI"

rmdir /q /s build

popd

build_submodule.bat