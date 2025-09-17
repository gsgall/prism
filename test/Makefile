GCOV_FLAGS = -fprofile-arcs -ftest-coverage
# flags for memory leakage testing

ASAN_FLAGS = -fsanitize=address,undefined,leak
# this flag marks something in the stl in for string hashing so we cannot use it right now
#ASAN_FLAGS += -fsanitize=integer
ASAN_FLAGS += -fsanitize=implicit-conversion,nullability
ASAN_FLAGS += -fsanitize=unsigned-integer-overflow
ASAN_FLAGS += -fsanitize=bounds,local-bounds,pointer-overflow
ASAN_FLAGS += -fsanitize=float-divide-by-zero,builtin
ASAN_FLAGS += -fno-omit-frame-pointer -fsanitize-address-use-after-scope

CXXFLAGS = -std=c++17 -g -Wall -Wextra -Wpedantic -Werror $(GCOV_FLAGS) $(ASAN_FLAGS) $(TESTING_FLAG)

PROJECT := prism
INCDIR := ../include
PROJECT_SRC_DIR := ../src
PROJECT_INCLUDES := -I$(INCDIR)/$(PROJECT)/core -I$(INCDIR)/$(PROJECT)/reactions

HELPER_SRC_DIR = helpers/src
HELPER_INCLUDE_DIR = helpers/include
TEST_SRC_DIR = tests
TEST_BUILD_DIR = build
TEST_EXECUTABLE = run_tests
TEST_INCLUDES := -I$(INCDIR)

CONDA_LIB_DIR = $(CONDA_PREFIX)/lib
CONDA_INCLUDE_DIR = $(CONDA_PREFIX)/include
GTEST_LIBS = -lgtest -lgtest_main
YAML_LINK = -lyaml-cpp
INPUTS_LINK = -linputs
TEST_LINKS = $(YAML_LINK) $(GTEST_LIBS) $(INPUTS_LINK)

PROJECT_SRC_SUBDIRS = reactions core
# Fix the variable references
PROJECT_SOURCES := $(wildcard $(PROJECT_SRC_DIR)/*.C) $(foreach subdir,$(PROJECT_SRC_SUBDIRS),$(wildcard $(PROJECT_SRC_DIR)/$(subdir)/*.C))
PROJECT_OBJECTS := $(patsubst $(PROJECT_SRC_DIR)/%.C,$(TEST_BUILD_DIR)/%.o,$(PROJECT_SOURCES))

HELPER_SOURCES := $(wildcard $(HELPER_SRC_DIR)/*.C)
HELPER_OBJECTS := $(patsubst $(HELPER_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(HELPER_SOURCES))
TEST_SOURCES := $(wildcard $(TEST_SRC_DIR)/*.C)
TEST_OBJECTS := $(patsubst $(TEST_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(TEST_SOURCES))
HELPER_INCLUDE := helpers/include

all: $(TEST_EXECUTABLE)

# building all of the projects files
$(TEST_BUILD_DIR)/%.o: $(PROJECT_SRC_DIR)/%.C
	@mkdir -p $(dir $@)  # Create subdirectories if needed
	@echo "Building $<"
	@$(CXX) $(CXXFLAGS) -I$(CONDA_INCLUDE_DIR) $(PROJECT_INCLUDES) -c $< -o $@
	@echo "Successfully Built $<"

# building all of the helper files
$(TEST_BUILD_DIR)/%.o: $(HELPER_SRC_DIR)/%.C
	@mkdir -p build
	@echo "Building $<"
	@$(CXX) $(CXXFLAGS) -I$(HELPER_INCLUDE) -I$(CONDA_INCLUDE_DIR) -c $< -o $@
	@echo "Successfully Built $<"

# building all of the tests
$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.C
	@mkdir -p build
	@echo "Building $<"
	@$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(PROJECT_INCLUDES) -I$(HELPER_INCLUDE) -I$(CONDA_INCLUDE_DIR) -c $< -o $@
	@echo "Successfully Built $<"

# linking everything into the executable
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(PROJECT_OBJECTS) $(HELPER_OBJECTS)
	@rm -rf build/*.g* build/*/*.g*
	@mkdir -p outputs
	@$(CXX) $(CXXFLAGS) -B$(CONDA_PREFIX)/bin $(TEST_INCLUDES) -I$(HELPER_INCLUDE) -I$(CONDA_INCLUDE_DIR) -L$(CONDA_LIB_DIR) $(TEST_LINKS) -o $@ $^ -Wl,-rpath,$(CONDA_PREFIX)/lib

.PHONY: all clean coverage clean-coverage

clean:
	@rm -rf $(TEST_BUILD_DIR)/*.o
	@rm -f $(TEST_EXECUTABLE)
	@rm -rf $(PROJECT_BUILD_DIR)/*.o
	@rm -rf coverage_report
	@rm -rf build/*
	@rm -rf *.dSYM
	@rm -f *.out
	@rm -f *.yaml
	@rm -f *.info
	@rm -rf outputs

coverage: all
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info "*/include/*" -o coverage.info
	lcov --remove coverage.info "*/gtest/*" -o coverage.info
	lcov --remove coverage.info "*/v1/*" -o coverage.info
	lcov --remove coverage.info "*/test/*" -o coverage.info
	lcov --remove coverage.info "*/yaml-cpp/*" -o coverage.info
	lcov --remove coverage.info "*/fmt/*" -o coverage.info
	genhtml coverage.info --output-directory coverage_report
	open coverage_report/index.html

clean-coverage:
	@rm -rf coverage.info
