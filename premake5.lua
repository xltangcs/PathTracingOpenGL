-- premake5.lua
workspace "PathTracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "PathTracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "External.lua"
include "PathTracing"