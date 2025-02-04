workspace "GameServer"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    -- Output directories
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "server_src"
    
    filter "configurations:Debug"
        symbols "On"
        defines {"DEBUG", "YOJIMBO_DEBUG", "NETCODE_DEBUG", "RELIABLE_DEBUG"}
    filter "configurations:Release"
        symbols "Off"
        optimize "On"
        defines {"YOJIMBO_RELEASE", "NETCODE_RELEASE", "RELIABLE_RELEASE"}
    filter "system:windows"
        defines {"OS_WINDOWS"}
    filter "system:linux"
        defines {"OS_LINUX"}

    -- Yojimbo Library
    project "Yojimbo"
        kind "StaticLib"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        -- Include all necessary files
        files {
            "../third_party/yojimbo/include/**.h",
            "../third_party/yojimbo/source/**.cpp",
            "../third_party/yojimbo/netcode/**.h",
            "../third_party/yojimbo/netcode/**.c",
            "../third_party/yojimbo/reliable/**.h",
            "../third_party/yojimbo/reliable/**.c",
            "../third_party/yojimbo/sodium/**.h",
            "../third_party/yojimbo/sodium/**.c",
            "../third_party/yojimbo/tlsf/**.h",
            "../third_party/yojimbo/tlsf/**.c"
        }

        includedirs {
            "../third_party/yojimbo/include",
            "../third_party/yojimbo/netcode",
            "../third_party/yojimbo/reliable",
            "../third_party/yojimbo/sodium",
            "../third_party/yojimbo/tlsf"
        }

    -- Solution
    project "server_src"
        kind "ConsoleApp"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files { "src/**.cpp", "src/**.hpp" }

        includedirs { "src" }

        libdirs { "bin/" .. outputdir .. "/Yojimbo" }
        links { "Yojimbo" }

        dependson { "Yojimbo" }  -- Ensure Yojimbo is built before MainApp

-- Custom Clean Action
newaction {
    trigger = "clean",
    description = "Remove all generated binaries and intermediate files",
    execute = function()
        print("Cleaning project...")
        os.rmdir("bin")
        os.rmdir("bin-int")
        os.remove("**.make")  -- Remove Makefiles
        os.remove("**.workspace")  -- Remove workspace files
        os.remove("**.sln")  -- Remove Visual Studio solution
        os.remove("**.vcxproj*")  -- Remove Visual Studio project files
        print("Clean complete!")
    end
}
