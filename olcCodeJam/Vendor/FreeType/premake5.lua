project "FreeType"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include;",
	}

	defines
	{
		"FT2_BUILD_LIBRARY",
		"_CRT_SECURE_NO_WARNINGS"
	}

    	files
    	{
            "freetype-gl/**.h",
            "freetype-gl/**.c",

            "include/**.h",
            "include/**.c",
            
        	"src/autofit/autofit.c",
            "src/bdf/bdf.c",
            "src/cff/cff.c",
            "src/base/ftbase.c",
            "src/base/ftbitmap.c",
            "src/cache/ftcache.c",
            "src/base/ftfstype.c",
            "src/base/ftglyph.c",
            "src/gzip/ftgzip.c",
            "src/base/ftinit.c",
            "src/lzw/ftlzw.c",
            "src/base/ftstroke.c",
            "src/base/ftsystem.c",
            "src/smooth/smooth.c",

            "src/base/ftbbox.c",
            "src/base/ftgxval.c",
            "src/base/ftlcdfil.c",
            "src/base/ftmm.c",
            "src/base/ftotval.c",
            "src/base/ftpatent.c",
            "src/base/ftpfr.c",
            "src/base/ftsynth.c",
            "src/base/ftxf86.c",
            "src/base/ftfstype.c",
            "src/pcf/pcf.c",
            "src/pfr/pfr.c",
            "src/psaux/psaux.c",
            "src/pshinter/pshinter.c",
            "src/psnames/psmodule.c",
            "src/raster/raster.c",
            "src/sfnt/sfnt.c",
            "src/truetype/truetype.c",
            "src/type1/type1.c",
            "src/cid/type1cid.c",
            "src/type42/type42.c",
            "src/winfonts/winfnt.c",      
    }

	filter "system:linux"
		pic "on"

		systemversion "latest"

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
