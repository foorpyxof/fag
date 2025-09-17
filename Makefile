.PHONY: all prep release debug libs test clean shaders

all: libs

CC != which clang 2>/dev/null
CCPLUS != which clang++ 2>/dev/null
AR != which ar

CC_WIN32 := x86_64-w64-mingw32-gcc
CCPLUS_WIN32 := x86_64-w64-mingw32-g++
AR_WIN32 := x86_64-w64-mingw32-ar

include make/early.mak

WINDOWS_TARGET_NAME := win64
LINUX_TARGET_NAME := linux

PREFIX := fag_
LIB_PREFIX := lib$(PREFIX)
DEBUG_SUFFIX := _debug

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
	
	EXE_EXT := .exe
	OBJ_EXT := .obj
	LIB_EXT := .lib

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

MKDIR_COMMAND = if ! [ -d "$(dir $@)" ]; then mkdir -p $(dir $@); fi

# $(1) is lib
define new-obj-target
$(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%,$(OBJECTS_RELEASE_C)): $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%$(OBJ_EXT): $(SOURCE_FOLDER)/$(1)/%.c
	$$(MKDIR_COMMAND)
	$(CC) $(CFLAGS) $$(alloc_new_rel_flags) -g -c $$< -o $$@

$(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%,$(OBJECTS_DEBUG_C)): $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%$(DEBUG_SUFFIX)$(OBJ_EXT): $(SOURCE_FOLDER)/$(1)/%.c
	$$(MKDIR_COMMAND)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $$< -o $$@

$(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%,$(OBJECTS_RELEASE_CPP)): $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%$(OBJ_EXT): $(SOURCE_FOLDER)/$(1)/%.cpp
	$$(MKDIR_COMMAND)
	$(CCPLUS) $(CPPFLAGS) $$(alloc_new_rel_flags) -c $$< -o $$@

$(filter $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%,$(OBJECTS_DEBUG_CPP)): $(OBJECTS_FOLDER)/$(1)/$(PREFIX)$(subst /,_,$(1))_%$(DEBUG_SUFFIX)$(OBJ_EXT): $(SOURCE_FOLDER)/$(1)/%.cpp
	$$(MKDIR_COMMAND)
	$(CCPLUS) $(CPPFLAGS) $(DEBUG_FLAGS) -c $$< -o $$@
endef

$(foreach lib,$(LIBRARY_NAMES),$(eval $(call new-obj-target,$(lib))))

# $(RELEASE_APP): LDFLAGS += -s
$(RELEASE_APP): $(MAIN_CPP) $(LIBS_RELEASE)
	@echo target: $(TARGET)
	if [[ "$(TARGET)" == "$(WINDOWS_TARGET_NAME)" ]]; then $(MAKE) $(REQUIRED_DLLS); fi
	$(CCPLUS) $(CPPFLAGS) $< \
	-L$(LIBRARY_FOLDER) $(foreach lib,$(filter-out $<,$^),-l$(patsubst lib%$(LIB_EXT),%,$(notdir $(lib)))) $(LDFLAGS) \
	$(EXTRA_FLAGS) $(RELEASE_FLAGS) -o $@

$(DEBUG_APP): $(MAIN_CPP) $(LIBS_DEBUG)
	@echo target: $(TARGET)
	if [[ "$(TARGET)" == "$(WINDOWS_TARGET_NAME)" ]]; then $(MAKE) $(REQUIRED_DLLS); fi
	$(CCPLUS) $(CPPFLAGS) $< \
	-L$(LIBRARY_FOLDER) $(foreach lib,$(filter-out $<,$^),-l$(patsubst lib%$(LIB_EXT),%,$(notdir $(lib)))) $(LDFLAGS) \
	$(EXTRA_FLAGS) $(DEBUG_FLAGS) -o $@

include make/zip.mak
