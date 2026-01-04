::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This file is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : copy_to_third_party.bat
::   
::   Content : Copying includes, binaries and licenses 
::		from External\\glfw to ThirdParty\\glfw
::
::=================================================

@echo off

xcopy /y /i /s "..\\..\\External\\glfw\\include" "..\\..\\ThirdParty\\glfw\\include" >nul
xcopy /y /i /s "..\\..\\External\\glfw\\build\\src\\MinSizeRel" "..\\..\\ThirdParty\\glfw\\win64" >nul

copy "..\\..\\External\\glfw\\LICENSE.md" "..\\..\\ThirdParty\\glfw\\GLFW_LICENSE.md" >nul
