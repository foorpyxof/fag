LIBRARY_NAMES := $(notdir $(shell find $(SOURCE_FOLDER) -mindepth 1 -maxdepth 1 -type d | sort))
LIBRARY_NAMES := $(subst /,_,$(LIBRARY_NAMES))

OBJECTS_FOLDER := $(BUILD_FOLDER)/objects
LIBRARY_FOLDER := $(BUILD_FOLDER)/lib


# $1 is library
# $2 is extension
define lib-src
SOURCE_$(2) += $(shell find $(SOURCE_FOLDER)/$(1) -type f -name "*.$(2)")
endef

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call lib-src,$(lib),c)) $(eval $(call lib-src,$(lib),cpp)))

# $1 is sources
# $2 is library
# $3 is extension
define fullsrc-to-components
COMPONENTS_$(3) += $(patsubst src_$(2)_%.cpp,$(2)/$(PREFIX)$(2)_%,$(filter src_$(2)_%,$(subst /,_,$(SOURCE_$(3)))))
endef

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call fullsrc-to-components,$(SOURCE_c),$(lib),c)))
$(foreach lib,$(LIBRARY_NAMES),$(eval $(call fullsrc-to-components,$(SOURCE_cpp),$(lib),cpp)))

# COMPONENTS_c := $(strip $(foreach lib,$(LIBRARY_NAMES),$(patsubst $(SOURCE_FOLDER)/$(lib)/%.c,$(lib)/$(PREFIX)$(subst /,_,$(lib))_%,$(filter-out %/test.c,$(SOURCE_c)))))

# COMPONENTS_cpp := $(strip $(foreach lib,$(LIBRARY_NAMES),$(patsubst $(SOURCE_FOLDER)/$(lib)/%.cpp,$(lib)/$(PREFIX)$(subst /,_,$(lib))_%,$(filter-out %/test.cpp,$(SOURCE_cpp)))))

OBJECTS_RELEASE_c := $(foreach c,$(COMPONENTS_c),$(OBJECTS_FOLDER)/$c$(OBJ_EXT))
OBJECTS_DEBUG_c := $(patsubst %$(OBJ_EXT),%$(DEBUG_SUFFIX)$(OBJ_EXT),$(OBJECTS_RELEASE_c))

OBJECTS_RELEASE_cpp := $(foreach c,$(COMPONENTS_cpp),$(OBJECTS_FOLDER)/$c$(OBJ_EXT))
OBJECTS_DEBUG_cpp := $(patsubst %$(OBJ_EXT),%$(DEBUG_SUFFIX)$(OBJ_EXT),$(OBJECTS_RELEASE_cpp))

OBJECTS_FOLDER := $(BUILD_FOLDER)/objects
LIBRARY_FOLDER := $(BUILD_FOLDER)/lib

COMPONENTS := $(COMPONENTS_c) $(COMPONENTS_cpp)

OBJECTS_RELEASE := $(strip $(OBJECTS_RELEASE_c) $(OBJECTS_RELEASE_cpp))
OBJECTS_DEBUG := $(strip $(OBJECTS_DEBUG_c) $(OBJECTS_DEBUG_cpp))

$(LIBRARY_FOLDER):
	mkdir -p $@

$(MODULES_DIR)/%:
	./scripts/update-submodules.sh

$(MODULES_DIR)/fpxlib3d/build/lib/%$(LIB_EXT): | $(MODULES_DIR)/fpxlib3d
	cd $|; $(MAKE) $(subst $(MODULES_DIR)/fpxlib3d/,,$@)

CORE_DEPS := vk window general
CORE_DEPS := $(foreach dep,$(CORE_DEPS),$(MODULES_DIR)/fpxlib3d/build/lib/libfpx3d_$(dep)$(LIB_EXT))

# for the Vulkan renderer
$(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PREFIX)core$(LIB_EXT): $(CORE_DEPS)

ifeq ($(FPXLIB3D_NODEBUG),)
$(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PREFIX)core$(DEBUG_SUFFIX)$(LIB_EXT): $(subst $(LIB_EXT),$(DEBUG_SUFFIX)$(LIB_EXT),$(CORE_DEPS))
else
$(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PREFIX)core$(DEBUG_SUFFIX)$(LIB_EXT): $(CORE_DEPS)
endif

LIB_CREATE_COMMAND = $(AR) cr --thin $$@ $$^ && echo -e 'create $$@\naddlib $$@\nsave\nend' | ar -M


define new-lib-target

LIBS_RELEASE += $($(1)_NAME_REL)
$($(1)_NAME_REL): $($(1)_OBJ_REL) | $(LIBRARY_FOLDER)
	-if [ -f $$@ ]; then rm $$@; fi
	$(LIB_CREATE_COMMAND)

LIBS_DEBUG += $($(1)_NAME_DEB)
$($(1)_NAME_DEB): $($(1)_OBJ_DBG) | $(LIBRARY_FOLDER)
	-if [ -f $$@ ]; then rm $$@; fi
	$(LIB_CREATE_COMMAND)

endef

FINAL_LIB_RELEASE := $(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PROJECT_NAME)$(LIB_EXT)
FINAL_LIB_DEBUG := $(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PROJECT_NAME)$(DEBUG_SUFFIX)$(LIB_EXT)

$(foreach lib,$(LIBRARY_NAMES),$(eval $(lib)_OBJ_REL := $(filter $(OBJECTS_FOLDER)/$(lib)/%,$(OBJECTS_RELEASE))) $(eval $(lib)_OBJ_DBG := $(filter $(OBJECTS_FOLDER)/$(lib)/%,$(OBJECTS_DEBUG))))

$(foreach lib,$(LIBRARY_NAMES),$(eval $(lib)_NAME_REL := $(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PREFIX)$(lib)$(LIB_EXT)) $(eval $(lib)_NAME_DEB := $(LIBRARY_FOLDER)/$(LIB_PFX_OPT)$(PREFIX)$(lib)$(DEBUG_SUFFIX)$(LIB_EXT)))

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call new-lib-target,$(lib))))

$(FINAL_LIB_RELEASE): $(LIBS_RELEASE)
	-if [ -f $@ ]; then rm $@; fi
	$(AR) cr --thin $@ $^ && echo -e 'create $@\naddlib $@\nsave\nend' | ar -M

$(FINAL_LIB_DEBUG): $(LIBS_DEBUG)
	-if [ -f $@ ]; then rm $@; fi
	$(AR) cr --thin $@ $^ && echo -e 'create $@\naddlib $@\nsave\nend' | ar -M
