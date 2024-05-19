workspace "Arcade"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Breakout"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["SDL2"] = "%{wks.location}/Core/Vendor/SDL2"

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Breakout/Build-App.lua"