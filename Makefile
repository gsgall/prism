WARNING_FLAGS = -Wall -Wextra -Wpedantic -Werror
CXXFLAGS = -std=c++17 $(WARNING_FLAGS) -g

# Source file and executable name
SRC = main.C
EXE = main
PROJECT = prism

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Subdirectories in src
SRC_SUBDIRS = reactions core

# Find all source files recursively in src and its subdirectories
SOURCES = $(wildcard $(SRCDIR)/*.C) $(foreach subdir,$(SRC_SUBDIRS),$(wildcard $(SRCDIR)/$(subdir)/*.C))

# Headers (assuming they might also be in subdirectories)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(INCDIR)/$(PROJECT)/*.h)

# Create object files maintaining directory structure
OBJECTS = $(patsubst $(SRCDIR)/%.C,$(BUILDDIR)/%.o,$(SOURCES))

# Adding include to look in the conda environment
INCLUDE_PATH = -isystem$(CONDA_PREFIX)/include

# Link instructions to look in the conda environment
LIBRARY_PATH = -L$(CONDA_PREFIX)/lib
YAML_LINK = -lyaml-cpp
INPUTS_LINK = -linputs
LINKS = $(YAML_LINK) $(INPUTS_LINK)

# Build both reaction parser and yaml library
all: $(EXE)

# Build just the reaction parser
$(EXE): $(OBJECTS)
	@echo "Building $(EXE)"
	@$(CXX) -B$(CONDA_PREFIX)/bin $(CXXFLAGS) $(INCLUDE_PATH) $(LIBRARY_PATH) -I$(INCDIR) $(OBJECTS) $(SRC) -o $(EXE) $(LINKS) -Wl,-rpath,$(CONDA_PREFIX)/lib
	@echo "$(EXE) built successfully!"

# Build all of the source files for the parser (handles subdirectories)
$(BUILDDIR)/%.o: $(SRCDIR)/%.C
	@mkdir -p $(@D)
	@echo "Building $<"
	@$(CXX) $(CXXFLAGS) -I$(INCDIR)/$(PROJECT)/core -I$(INCDIR)/$(PROJECT)/reactions $(INCLUDE_PATH) -c $< -o $@
	@echo "$< built successfully"

# Clean up the parser
clean:
	@rm -rf *.dSYM
	@rm -f $(EXE)
	@rm -rf build

# Debug target to show what files are found (optional - for troubleshooting)
debug:
	@echo "Source files found:"
	@echo $(SOURCES)
	@echo ""
	@echo "Header files found:"
	@echo $(HEADERS)
	@echo ""
	@echo "Object files will be:"
	@echo $(OBJECTS)
