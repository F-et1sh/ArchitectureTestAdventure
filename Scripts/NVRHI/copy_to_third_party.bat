::=================================================
::
::   Copyright (C) 2025 Farrakh.  All Rights Reserved.
::   
::   This source is a part of ArchitectureTestAdventure.
::   Check README.md for more information.
::   
::   File : copy_to_third_party.bat
::   
::   Content : Copying includes, binaries and licenses 
::		from External\\NVRHI to ThirdParty\\NVRHI
::
::=================================================

@echo off

xcopy /y /i /s "..\\..\\External\\NVRHI\\include" "..\\..\\ThirdParty\\NVRHI\\include" >nul
xcopy /y /i /s "..\\..\\External\\NVRHI\\build\\Release" "..\\..\\ThirdParty\\NVRHI\\win64" >nul

copy "..\\..\\External\\NVRHI\\rtxmu\\LICENSE.txt" "..\\..\\ThirdParty\\NVRHI\\NVRHI_RTXMU_LICENSE.txt" >nul
copy "..\\..\\External\\NVRHI\\thirdparty\\DirectX-Headers\\LICENSE" "..\\..\\ThirdParty\\NVRHI\\NVRHI_DIRECT_X_LICENSE.txt" >nul
copy "..\\..\\External\\NVRHI\\thirdparty\\Vulkan-Headers\\LICENSES\\MIT.txt" "..\\..\\ThirdParty\\NVRHI\\NVRHI_VULKAN_LICENSE_MIT.txt" >nul

copy "..\\..\\External\\NVRHI\\LICENSE.txt" "..\\..\\ThirdParty\\NVRHI\\NVRHI_LICENSE.txt" >nul
copy "..\\..\\External\\NVRHI\\ThirdPartyLicenses.txt" "..\\..\\ThirdParty\\NVRHI\\NVRHI_ThirdPartyLicenses.txt" >nul
