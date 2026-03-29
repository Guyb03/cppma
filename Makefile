BUILD_DIR := build
VTK_DIR   := vtk

.PHONY: all build test clean clean-build clean-vtk re

all: build

build:
	@if [ ! -f $(BUILD_DIR)/CMakeCache.txt ]; then cmake -B $(BUILD_DIR); fi
	cmake --build $(BUILD_DIR) --parallel

test: build
	cd $(BUILD_DIR) && ctest --output-on-failure

clean-build:
	rm -rf $(BUILD_DIR)

clean-vtk:
	find $(VTK_DIR) -name "*.vtu" -delete 2>/dev/null || true

clean: clean-build clean-vtk

re: clean-build
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) --parallel
