LIBRARY_NAMES := core os error

OBJECTS_FOLDER := $(BUILD_FOLDER)/objects
LIBRARY_FOLDER := $(BUILD_FOLDER)/lib

COMPONENTS_C := $(strip $(foreach lib,$(LIBRARY_NAMES),$(patsubst $(SOURCE_FOLDER)/$(lib)/%.c,$(lib)/$(PREFIX)$(subst /,_,$(lib))_%,$(filter-out %/test.c,$(wildcard $(SOURCE_FOLDER)/$(lib)/*.c)))))

COMPONENTS_CPP := $(strip $(foreach lib,$(LIBRARY_NAMES),$(patsubst $(SOURCE_FOLDER)/$(lib)/%.cpp,$(lib)/$(PREFIX)$(subst /,_,$(lib))_%,$(filter-out %/test.cpp,$(wildcard $(SOURCE_FOLDER)/$(lib)/*.cpp)))))

OBJECTS_RELEASE_C := $(foreach c,$(COMPONENTS_C),$(OBJECTS_FOLDER)/$c$(OBJ_EXT))
OBJECTS_DEBUG_C := $(patsubst %$(OBJ_EXT),%$(DEBUG_SUFFIX)$(OBJ_EXT),$(OBJECTS_RELEASE_C))

OBJECTS_RELEASE_CPP := $(foreach c,$(COMPONENTS_CPP),$(OBJECTS_FOLDER)/$c$(OBJ_EXT))
OBJECTS_DEBUG_CPP := $(patsubst %$(OBJ_EXT),%$(DEBUG_SUFFIX)$(OBJ_EXT),$(OBJECTS_RELEASE_CPP))

OBJECTS_FOLDER := $(BUILD_FOLDER)/objects
LIBRARY_FOLDER := $(BUILD_FOLDER)/lib

COMPONENTS := $(COMPONENTS_C) $(COMPONENTS_CPP)

OBJECTS_RELEASE := $(strip $(OBJECTS_RELEASE_C) $(OBJECTS_RELEASE_CPP))
OBJECTS_DEBUG := $(strip $(OBJECTS_DEBUG_C) $(OBJECTS_DEBUG_CPP))

$(foreach lib,$(LIBRARY_NAMES),$(eval $(lib)_OBJ_REL := $(filter $(OBJECTS_FOLDER)/$(lib)/%,$(OBJECTS_RELEASE))))
$(foreach lib,$(LIBRARY_NAMES),$(eval $(lib)_OBJ_DBG := $(filter $(OBJECTS_FOLDER)/$(lib)/%,$(OBJECTS_DEBUG))))

$(LIBRARY_FOLDER):
	mkdir -p $@

$(MODULES_DIR)/%:
	./scripts/update-submodules.sh

$(MODULES_DIR)/fpxlib3d/build/lib/%$(LIB_EXT): | $(MODULES_DIR)/fpxlib3d
	cd $|; $(MAKE) $(subst $(MODULES_DIR)/fpxlib3d/,,$@)

CORE_DEPS := vk window general
CORE_DEPS := $(foreach dep,$(CORE_DEPS),$(MODULES_DIR)/fpxlib3d/build/lib/libfpx3d_$(dep)$(LIB_EXT))

# for the Vulkan renderer
$(LIBRARY_FOLDER)/$(LIB_PREFIX)core$(LIB_EXT): $(CORE_DEPS)
$(LIBRARY_FOLDER)/$(LIB_PREFIX)core$(DEBUG_SUFFIX)$(LIB_EXT): $(subst $(LIB_EXT),$(DEBUG_SUFFIX)$(LIB_EXT),$(CORE_DEPS))

define new-lib-target

LIBS_RELEASE += $(LIBRARY_FOLDER)/$(LIB_PREFIX)$(1)$(LIB_EXT)
$(LIBRARY_FOLDER)/$(LIB_PREFIX)$(1)$(LIB_EXT): $($(1)_OBJ_REL) | $(LIBRARY_FOLDER)
	-if [ -f $$@ ]; then rm $$@; fi
	$(AR) cr --thin $$@ $$^ && echo -e 'create $$@\naddlib $$@\nsave\nend' | ar -M

LIBS_DEBUG += $(LIBRARY_FOLDER)/$(LIB_PREFIX)$(1)$(DEBUG_SUFFIX)$(LIB_EXT)
$(LIBRARY_FOLDER)/$(LIB_PREFIX)$(1)$(DEBUG_SUFFIX)$(LIB_EXT): $($(1)_OBJ_DBG) | $(LIBRARY_FOLDER)
	-if [ -f $$@ ]; then rm $$@; fi
	$(AR) cr --thin $$@ $$^ && echo -e 'create $$@\naddlib $$@\nsave\nend' | ar -M

endef

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call new-lib-target,$(lib))))
