#pragma once

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

// Will be automatically increased by AutoIncreaseBuildNrs.exe
// cmd: AutoIncreaseBuildNrs.exe version.h
<<<<<<< HEAD:Lib/API/version.h
#define VER_BUILDNR 46
// Will be automatically increased by get-git-rev-HEAD.py
// cmd: python get-git-rev-HEAD.py $(MSBuildProjectDir) version.h
#define VER_BUILD					dd93bb8
=======
#define VER_BUILDNR 1
// Will be automatically increased by get-git-rev-HEAD.py
// cmd: python get-git-rev-HEAD.py $(MSBuildProjectDir) version.h
#define VER_BUILD					dd93bb8
>>>>>>> parent of 68aad79... Move files from Lib to Lib/API:Lib/version.h


#define VERSION_MAJOR				0
#define VERSION_MINOR				3
#define VERSION_REVISION			VER_BUILDNR
#define VERSION_BUILD				VER_BUILD

#define VER_FILE_DESCRIPTION_STR	"Rexroth Indradrive API"
#define VER_FILE_VERSION			VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION
#define VER_FILE_VERSION_STR		STRINGIZE(VERSION_MAJOR)     \
									   "." STRINGIZE(VERSION_MINOR) \
									   "." STRINGIZE(VERSION_REVISION) \
									   "." STRINGIZE(VERSION_BUILD) \

#define VER_PRODUCTNAME_STR			"Indradrive Wrapper"
#define VER_PRODUCT_VERSION			VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR		VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   "IndradriveWrapper.dll"
#define VER_INTERNAL_NAME_STR		VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR			"Copyright (C) Infineon Technologies AG 2016"

#ifdef _DEBUG
#define VER_VER_DEBUG				VS_FF_DEBUG
#else
#define VER_VER_DEBUG				0
#endif

#define VER_FILEOS					VOS_NT_WINDOWS32
#define VER_FILEFLAGS				VER_VER_DEBUG
#define VER_FILETYPE				VFT_DLL
