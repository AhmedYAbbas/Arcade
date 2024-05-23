project "Arcade"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        "Source",

        -- Include Core
        "../Core/Source",
        "../Core/Vendor",
        "%{IncludeDir.SDL2}/include"
    }

    links
    {
        "Core"
    }

    postbuildcommands
    {
        "{COPY} ../Core/Vendor/SDL2/lib/x64/SDL2.dll ../Binaries/" .. OutputDir .. "/%{prj.name}",
        "{COPY} Assets ../Binaries/" .. OutputDir .. "/%{prj.name}"
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }

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