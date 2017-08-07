ifneq ($(filter qacclean qacreport qac,$(MAKECMDGOALS)), )
include qac_analysis.mk
endif

ifeq ($(filter clean, $(MAKECMDGOALS)), )
ifneq ($(wildcard objects.txt),)
objects_txt_content := $(shell cat objects.txt) 
endif
ifneq ($(wildcard include_paths.txt),) 
include_paths_txt_content := $(subst -I,,$(shell cat include_paths.txt))
endif

ifeq ($(filter-out $(objects_txt_content),$(OBJECTS)),)
ifeq ($(filter-out $(OBJECTS), $(objects_txt_content)),)
objects_match = 1
endif
endif

.PHONY: force_objects_del objects.txt include_paths.txt force_include_paths_del
ifndef objects_match
force_objects_del:
	@echo "Generating objects.txt ..."
	@rm -f objects.txt
objects.txt: force_objects_del
	$(foreach var,$(OBJECTS),$(shell echo $(var) >>  $@))
else
objects.txt:
	@echo "Nothing to generate for $@"
endif

ifneq ($(include_paths_txt_content), $(sort . $(SOURCEDIRS) $(EXTRAINCDIRS)))
force_include_paths_del:
	@echo "Generating include_paths.txt ..."
	@rm -f include_paths.txt
include_paths.txt: force_include_paths_del
	$(foreach var,$(sort . $(SOURCEDIRS) $(EXTRAINCDIRS)),$(shell echo -I$(var) >> $@))
else
include_paths.txt:
	@echo "Nothing to generate for $@"
endif
endif