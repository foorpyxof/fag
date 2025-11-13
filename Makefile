.PHONY: all prep release debug libs test clean shaders

all: libs

PROJECT_NAME := fag
PREFIX := $(PROJECT_NAME)_
LIB_PFX_OPT := lib
DEBUG_SUFFIX := _debug

include make/early.mak

CC != which clang 2>/dev/null
CCPLUS != which clang++ 2>/dev/null
AR != which ar

CC_WIN32 != which x86_64-w64-mingw32-gcc 2>/dev/null
CCPLUS_WIN32 != which x86_64-w64-mingw32-g++ 2>/dev/null
AR_WIN32 != which x86_64-w64-mingw32-ar 2>/dev/null

WINDOWS_TARGET_NAME := win64
LINUX_TARGET_NAME := linux

ifeq ($(WINDOWS),true)
	TARGET := $(WINDOWS_TARGET_NAME)
else
	TARGET := $(LINUX_TARGET_NAME)
endif

include make/*.mk

ifeq ($(TARGET),$(WINDOWS_TARGET_NAME))

	-include make/windows/*.mk

	CC := $(CC_WIN32)
	CCPLUS := $(CCPLUS_WIN32)
	AR := $(AR_WIN32)
	CFLAGS += -mwindows
	CPPFLAGS += -mwindows

	# mingw/bin/libwinpthread.dll.a import library
	LDFLAGS += -lwinpthread.dll
	LDFLAGS += -lglfw3
	
	EXE_EXT := .exe
	OBJ_EXT := .obj
	LIB_EXT := .lib

	undefine LIB_PFX_OPT

else

ifeq ($(CC),)
	CC != which cc
endif
ifeq ($(CCPLUS),)
	CC != which g++
endif

	LDFLAGS += -lglfw
	
	# EXE_EXT := .out
	OBJ_EXT := .o
	LIB_EXT := .a

endif

EXE_EXT := $(TARGET)$(EXE_EXT)

include make/variables.mak
include make/dll.mak
include make/library.mak

clean:
	rm -rf ./$(BUILD_FOLDER) || true

release: $(LIBS_RELEASE)
debug: $(LIBS_DEBUG)

# testing app
test: $(RELEASE_APP) $(DEBUG_APP) $(SHADER_FILES)

# individual libraries, both RELEASE and DEBUG
libs: $(LIBS_RELEASE) $(LIBS_DEBUG)

$(OBJECTS_FOLDER):
	mkdir -p $@

MKDIR_COMMAND = @mkdir -p $(dir $@)
# MKDIR_COMMAND = if ! [ -d "$(dir $@)" ]; then mkdir -p $(dir $@); fi

# $(1) is object file
# $(2) is source file
define new-rel-c
$(1): $(2)
	$$(MKDIR_COMMAND)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $$< -o $$@
endef
define new-dbg-c
$(1): $(2)
	$$(MKDIR_COMMAND)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $$< -o $$@
endef
define new-rel-cpp
$(1): $(2)
	$$(MKDIR_COMMAND)
	$(CCPLUS) $(CPPFLAGS) $(RELEASE_FLAGS) -c $$< -o $$@
endef
define new-dbg-cpp
$(1): $(2)
	$$(MKDIR_COMMAND)
	$(CCPLUS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $$< -o $$@
endef

# $(1) is lib
define new-lib-target

$(1)_rel_c := $(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%,$(OBJECTS_RELEASE_c))
$(1)_dbg_c := $(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%,(OBJECTS_DEBUG_c))

$(1)_rel_cpp := $(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%,$(OBJECTS_RELEASE_cpp))
$(1)_dbg_cpp := $(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%,$(OBJECTS_DEBUG_cpp))

$$(foreach rel_c,$$($(1)_rel_c),$$(eval $$(call new-rel-c,$$(rel_c),$$(subst _,/,$$(patsubst $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%$(OBJ_EXT),$(SOURCE_FOLDER)/$(1)/%.c,$$(rel_c))))))
$$(foreach dbg_c,$$($(1)_dbg_c),$$(eval $$(call new-dbg-c,$$(dbg_c),$$(subst _,/,$$(patsubst $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%$(DEBUG_SUFFIX)$(OBJ_EXT),$(SOURCE_FOLDER)/$(1)/%.c,$$(dbg_c))))))
$$(foreach rel_cpp,$$($(1)_rel_cpp),$$(eval $$(call new-rel-cpp,$$(rel_cpp),$$(subst _,/,$$(patsubst $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%$(OBJ_EXT),$(SOURCE_FOLDER)/$(1)/%.cpp,$$(rel_cpp))))))
$$(foreach dbg_cpp,$$($(1)_dbg_cpp),$$(eval $$(call new-dbg-cpp,$$(dbg_cpp),$$(subst _,/,$$(patsubst $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(1)_%$(DEBUG_SUFFIX)$(OBJ_EXT),$(SOURCE_FOLDER)/$(1)/%.cpp,$$(dbg_cpp))))))

endef

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call new-lib-target,$(lib))))

# $(RELEASE_APP): LDFLAGS += -s
$(RELEASE_APP): $(MAIN_CPP) $(FINAL_LIB_RELEASE)
	@echo target: $(TARGET)
	if [[ "$(TARGET)" == "$(WINDOWS_TARGET_NAME)" ]]; then $(MAKE) $(REQUIRED_DLLS); fi
	$(CCPLUS) $(CPPFLAGS) $< \
	-L$(LIBRARY_FOLDER) $(foreach lib,$(filter-out $<,$^),-Wl,"$(lib)") $(LDFLAGS) \
	$(EXTRA_FLAGS) $(RELEASE_FLAGS) -o $@

$(DEBUG_APP): $(MAIN_CPP) $(FINAL_LIB_DEBUG)
	@echo target: $(TARGET)
	if [[ "$(TARGET)" == "$(WINDOWS_TARGET_NAME)" ]]; then $(MAKE) $(REQUIRED_DLLS); fi
	$(CCPLUS) $(CPPFLAGS) $< \
	-L$(LIBRARY_FOLDER) $(foreach lib,$(filter-out $<,$^),-Wl,"$(lib)") $(LDFLAGS) \
	$(EXTRA_FLAGS) $(DEBUG_FLAGS) -o $@

include make/zip.mak
