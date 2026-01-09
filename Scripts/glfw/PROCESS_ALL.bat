::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This file is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : PROCESS_ALL.bat
::   
::   Content : run all scripts in queue
::
::=================================================

@echo off

call build_submodule_debug.bat
call build_submodule_release.bat
call build_submodule_min_size_rel.bat
call copy_to_third_party.bat