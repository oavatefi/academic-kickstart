# List common C source files here.

project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

CSRC :=
CXXSRC :=  
EXTRAINCDIRS = $(project_path) $(project_path)/daimler_src