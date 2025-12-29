::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This source is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : update_submodules.bat
::   
::   Content : Update all submodules
::
::=================================================

pushd "%~dp0\..\"
git submodule update --init --recursive
popd