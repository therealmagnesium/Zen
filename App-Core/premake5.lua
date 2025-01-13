local outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project("App-Core")
kind("StaticLib")
language("C++")
cppdialect("C++17")

targetdir("../bin/" .. outputdir .. "/%{prj.name}")
objdir("../build/" .. outputdir .. "/%{prj.name}")

files({
	"source/**.h",
	"source/**.cpp",
})

includedirs({
	"source",
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
	"SDL2",
	"glad",
	"imgui",
	"stb_image",
	"nfd",
	"gtk-3",
	"glib-2.0",
	"assimp",
	"yaml-cpp",
})

filter("system:Unix")
system("linux")
systemversion("latest")
defines({
	"PLATFORM_LINUX",
})

filter({ "configurations:Debug" })
defines("DEBUG")
symbols("on")

filter({ "configurations:Release" })
defines("RELEASE")
optimize("on")

filter({ "configurations:Dist" })
defines("DIST")
optimize("on")
