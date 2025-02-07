local outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project("Application")
kind("ConsoleApp")
language("C++")
cppdialect("C++17")
systemversion("latest")

targetdir("../bin/" .. outputdir .. "/%{prj.name}")
objdir("../build/" .. outputdir .. "/%{prj.name}")

files({
    "source/**.h",
    "source/**.cpp",
})

includedirs({
    "../App-Core/source",
    "../vendor/glm",
    "../vendor/SDL2/include",
    "../vendor/glad/include",
    "../vendor/imgui/include",
    "../vendor/nfd/include",
    "../vendor/stb_image/include",
    "../vendor/yaml-cpp/include",
})

libdirs({
    "../vendor/SDL2/lib",
    "../vendor/glad/lib",
    "../vendor/imgui/lib",
    "../vendor/nfd/lib",
    "../vendor/stb_image/lib",
    "../vendor/yaml-cpp/lib",
})

links({
    "App-Core",
    "SDL2",
    "glad",
    "imgui",
    "nfd",
    "stb_image",
    "assimp",
    "yaml-cpp",
})

postbuildcommands({
    "cp -r assets/ %{cfg.buildtarget.directory}",
    "cp -r scenes/ %{cfg.buildtarget.directory}",
    "cp ../imgui.ini %{cfg.buildtarget.directory}",
})

filter("system:linux")
links({
    "gtk-3",
    "glib-2.0",
    "gobject-2.0",
})
defines({
    "PLATFORM_LINUX",
})

filter("system:windows")
links({
    "comctl32",
})
defines({
    "PLATFORM_WINDOWS",
})

filter("configurations:Debug")
defines("DEBUG")
symbols("on")

filter("configurations:Release")
defines("RELEASE")
optimize("on")

filter("configurations:Dist")
defines("DIST")
optimize("on")
kind("WindowedApp")
