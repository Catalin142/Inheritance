workspace "olcCodeJam"
	architecture "x86"
	configurations { "Debug", "Release" }
	startproject "Sandbox"
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	IncludeDir = {}
	IncludeDir["GLEW"] = "%{wks.location}/olcCodeJam/Vendor/GLEW/include"
	IncludeDir["GLFW"] = "%{wks.location}/olcCodeJam/Vendor/GLFW/include"
	IncludeDir["stb_image"] = "%{wks.location}/olcCodeJam/Vendor/stb_image"
	IncludeDir["glm"] = "%{wks.location}/olcCodeJam/Vendor/glm"
	IncludeDir["FreeType"] = "%{wks.location}/olcCodeJam/Vendor/FreeType/include"
	
	group "Dependencies"
	include "olcCodeJam/Vendor/GLEW"
	include "olcCodeJam/Vendor/GLFW"
	include "olcCodeJam/Vendor/FreeType"
	group ""

	include "olcCodeJam"
