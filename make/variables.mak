RELEASE_FLAGS := -DNDEBUG -O3
DEBUG_FLAGS := -DDEBUG -g -O0 -DFAG_DEBUG_ENABLE

# DEBUG_FLAGS += -fsanitize=address
# ^^^ uncomment for ASAN


# !!! ADD THESE VARIABLES ON YOUR OWN IF NECESSARY (FOR EXAMPLE WHEN CROSS COMPILING) !!!
EXTRA_INCLUDE_DIRS += $(GLFW_INCLUDE_DIRECTORY) $(CGLM_INCLUDE_DIRECTORY)
EXTRA_LIB_DIRS += $(GLFW_LIBRARY_DIRECTORY)


# some folders
BUILD_FOLDER := build
SOURCE_FOLDER := src
MODULES_DIR := modules

INCLUDE_DIRS := include $(MODULES_DIR) $(EXTRA_INCLUDE_DIRS)
LIB_DIRS := $(EXTRA_LIB_DIRS)

# for Vulkan headers; we're using the same headers as the Vulkan library we're using
INCLUDE_DIRS += $(MODULES_DIR)/fpxlib3d/modules/Vulkan-Headers/include

# comp/link flags
CFLAGS += $(foreach dir,$(INCLUDE_DIRS),-I$(dir))
CPPFLAGS += $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

LDFLAGS += $(foreach dir,$(LIB_DIRS),-L$(dir))

# some file name definitions, for targets
RELEASE_APP := $(BUILD_FOLDER)/release-$(EXE_EXT)
DEBUG_APP := $(BUILD_FOLDER)/debug-$(EXE_EXT)

MAIN_CPP := $(SOURCE_FOLDER)/main.cpp
