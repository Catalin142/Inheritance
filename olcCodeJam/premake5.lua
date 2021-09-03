project "olcCodeJam"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"%{wks.location}/olcCodeJam/Source",
		"%{wks.location}/olcCodeJam/Vendor",
	}

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Vendor/stb_image/**.h",
		"Vendor/stb_image/**.cpp",
		"Vendor/glm/**.hpp",
		"Vendor/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"AL_LIBTYPE_STATIC"
	}

	includedirs
	{
		"Vendor;",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"Vendor/stb_image",
		"Vendor/FreeType/include",
		"Source;",
		"Vendor/glm",
	}

	links 
	{
		"GLFW",
		"GLEW",
		"FreeType",
		"opengl32.lib",
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG=1"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE=1"
		runtime "Release"
		optimize "on"