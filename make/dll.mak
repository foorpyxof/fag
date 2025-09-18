MINGW_DLLS := libwinpthread-1.dll libgcc_s_seh-1.dll libstdc++-6.dll
GLFW_DLL := glfw3.dll

REQUIRED_DLLS := $(MINGW_DLLS) $(GLFW_DLL)

find_pipe := head -n 1 | xargs -I found ln -s found .

$(MINGW_DLLS):
ifeq ($(MINGW_DIRECTORY),)
	$(error MINGW_DIRECTORY variable not set)
endif
	find $(MINGW_DIRECTORY) -name "$@" | $(find_pipe)

$(GLFW_DLL):
ifeq ($(GLFW_LIBRARY_DIRECTORY),)
	$(error GLFW_LIBRARY_DIRECTORY variable not set)
endif
	find $(GLFW_LIBRARY_DIRECTORY) -name "$@" | $(find_pipe)
