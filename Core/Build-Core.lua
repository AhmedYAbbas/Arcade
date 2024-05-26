project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    pchheader "CorePCH.h"
    pchsource "Source/CorePCH.cpp"

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
      "Source",
      "%{IncludeDir.SDL2}/include"
    }

    libdirs
    {
        "%{IncludeDir.SDL2}/lib/x64"
    }

    links 
    {
        "SDL2",
        "SDL2main"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"