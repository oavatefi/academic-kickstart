# Try to determine where this file is located.  If the caller did
# include /foo/... then extract the /foo/ so that common gets
# included transparently
common_root :=
ifneq ($(MAKEFILE_LIST),)
 common_root := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
endif

include $(common_root)common.mk
include $(common_root)config_build.mk

ifndef OBJEXT
  OBJEXT:=.o
endif

ifndef DEPEXT
  DEPEXT:=.d
endif

# Create some sets of source files (CSRC, CSRC1 .. CSRC9, ASRC, ASRC1 .. ASRC9)
$(eval $(call src_template,,CSOURCES,COBJECTS,CSRC,COBJ,CFLAGS,$(OBJEXT)))
$(foreach srcset,1 2 3 4 5 6 7 8 9,$(eval $(call src_template,$(srcset),CSOURCES,COBJECTS,CSRC,COBJ,CFLAGS,$(OBJEXT))))
$(eval $(call src_template,,CXXSOURCES,CXXOBJECTS,CXXSRC,CXXOBJ,CXXFLAGS,$(OBJEXT)))
$(foreach srcset,1 2 3 4 5 6 7 8 9,$(eval $(call src_template,$(srcset),CXXSOURCES,CXXOBJECTS,CXXSRC,CXXOBJ,CXXFLAGS,$(OBJEXT))))
$(eval $(call src_template,,ASOURCES,AOBJECTS,ASRC,AOBJ,ASFLAGS,$(OBJEXT)))
$(foreach srcset,1 2 3 4 5 6 7 8 9,$(eval $(call src_template,$(srcset),ASOURCES,AOBJECTS,ASRC,AOBJ,ASFLAGS,$(OBJEXT))))

# Target files
TARGETFILES:=$(foreach ext,$(TARGET_EXT),$(call outdirsuffix,$(TARGET),$(ext)))

# Dependency files
DEPFILES:=$(foreach sourcefile,$(CSOURCES),$(call objdirsuffix,$(OBJ_$(sourcefile)),$(DEPEXT)))
DEPFILES += $(foreach sourcefile,$(CXXSOURCES),$(call objdirsuffix,$(OBJ_$(sourcefile)),$(DEPEXT)))
# Temporary files created during build
TEMPFILES:=$(addprefix $(call outdirsuffix,$(TARGET),),$(TMPEXT_TARG)) \
	$(foreach file,$(call objdirsuffix,$(CSOURCES),),$(addprefix $(file),$(TMPEXT_C))) \
	$(foreach file,$(call objdirsuffix,$(ASOURCES),),$(addprefix $(file),$(TMPEXT_ASM))) \
	$(GARBAGE) $(OBJECTS) $(DEPFILES) $(TARGETFILES)

# Build only the dependency files
deps: $(DEPFILES)

# Clean project.
clean:
	@$(ECHO) "Cleaning project"
	$(REMOVE) $(call outdirsuffix,*,)
	$(REMOVE) $(call objdirsuffix,*,)
	$(REMOVE) $(GARBAGE)

# Clean all dependency files
cleandeps:
	$(REMOVE) $(DEPFILES)
	
# Show the source files used to build this configuration
show_src:
	@$(ECHO)
	@$(ECHO) "SOURCES: $(CSOURCES) $(ASOURCES)"

# Steps to perform before each build
$(PREDEPS): | $(OUTDIR)
prebuild: checkdirs $(OBJDIR) $(PREDEPS)

# Steps to perform before linking
prelink:

# Check the output directory and the object directory
checkdirs:
ifneq ($(OBJDIR),$(OUTDIR))
	@if [ -e $(OBJDIR) -a ! -d $(OBJDIR) ]; then \
		$(ECHO) "A file $(OBJDIR) is blocking the creation of the object directory!"; \
		exit 1; \
	fi
endif
	@if [ -e $(OUTDIR) -a ! -d $(OUTDIR) ]; then \
		$(ECHO) "A file $(OUTDIR) is blocking the creation of the output directory!"; \
		exit 1; \
	fi
	
ifneq ($(OBJDIR),$(OUTDIR))
$(OBJDIR):
	@$(ECHO)
	@$(ECHO) "Creating object directory: $@"
	@$(MKDIR) $@
endif

$(OUTDIR):
	@$(ECHO)
	@$(ECHO) "Creating output directory: $@"
	@$(MKDIR) $@

# Some make magic: dynamically define static rules for object files. Unlike using static
# pattern rules it is possible to determine the source files by their full path. So the
# build system does not stumble if there are source files with the same name in the project.
define rule_template
$$(call objdirsuffix,$$(OBJ_$(1)),$(2)) : $(1)
endef
$(foreach source,$(CSOURCES),$(eval $(call rule_template,$(source),$(OBJEXT))))
$(foreach source,$(CSOURCES),$(eval $(call rule_template,$(source),$(DEPEXT))))
$(foreach source,$(CXXSOURCES),$(eval $(call rule_template,$(source),$(OBJEXT))))
$(foreach source,$(CXXSOURCES),$(eval $(call rule_template,$(source),$(DEPEXT))))
$(foreach source,$(ASOURCES),$(eval $(call rule_template,$(source),$(OBJEXT))))

# Do prebuild steps
$(OBJECTS): | prebuild

# Compile: create object files from C source files.
$(COBJECTS) :
	@$(ECHO)
	@$(ECHO) "Compiling: $<"
	$(call RUN_CC,$<,$@)

# Compile: create object files from CXX source files.
$(CXXOBJECTS) :
	@$(ECHO) "Compiling: $<"
	$(call RUN_CXX,$<,$@)

# Assemble: create object files from assembler source files.
$(AOBJECTS) :
	@$(ECHO)
	@$(ECHO) "Assembling: $<"
	$(call RUN_AS,$<,$@)

# Link
$(TARGETFILES): $(OBJECTS) | prelink
	@$(ECHO)
	@$(ECHO) "Linking: $@"
	$(call RUN_LD,$^,$@)

# Build the dependencies. In the first step the dependencies are created by MKDEPS in the form
#   target.o: dep1 dep2 dep3...
# Then the dependency file itself is added to the targets, so that the depfile is rebuilt if a dependency changes.
#   target.d target.o: dep1 dep2 dep3...
# The last step adds for each dependency an empty target to the depfile, so that there is no make error thrown if
# a dependency was deleted but the depfile is rebuilt.
#   target.d target.o: dep1 dep2 dep3...
#   dep1:
#   dep2:
#   ...
# If there is an error during dependency generation the depfile is empty and will be deleted.
MAKEFILES_wo_dep := $(MAKEFILE_LIST)
$(DEPFILES) : | prebuild
ifndef SKIP_DEP_GEN
$(DEPFILES) : 
	@$(ECHO) "Updating dependencies of $<"
	@$(MKDEPS) $(CDEFS) $(GENDEPFLAGS) $< | $(SED) -e 's|\(^\S\+\)\.o:|$@ $(OBJDIR)/\1$(OBJEXT):$(MAKEFILES_wo_dep) $(PREDEPS)|' | $(SED) -e 's|\(^\S\+\)\.o:|$@ $(OBJDIR)/\1$(OBJEXT):*.mk $(PREDEPS)|'> $@
	@[ -s $@ ] || $(REMOVE) $@
endif

# Include the dependency files.
ifneq ($(CSOURCES),)
 ifeq ($(filter $(CUSTOM_RULES) clean, $(MAKECMDGOALS)), )
  ifneq ($(MAKECMDGOALS),doxygen)
   -include $(DEPFILES)
  endif
 endif
endif

# Listing of phony targets.
.PHONY : clean deps cleandeps show_src prebuild prelink checkdirs
