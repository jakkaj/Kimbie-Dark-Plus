# Makefile for Kimbie Dark ++ VS Code Theme

# Dynamically determine package name and version from package.json
# Requires node to be installed and accessible in the PATH
PACKAGE_NAME := $(shell node -p "require('./package.json').name")
PACKAGE_VERSION := $(shell node -p "require('./package.json').version")
VSIX_FILE := $(PACKAGE_NAME)-$(PACKAGE_VERSION).vsix

# Phony targets prevent conflicts with files of the same name
.PHONY: build clean install default

# Target to build the theme package by running the script
build:
	@echo "Running packaging script..."
	@./package.sh

# Target to install the generated VSIX package into VS Code
install: build
	@echo "Installing $(VSIX_FILE)..."
	@code --install-extension $(VSIX_FILE) || (echo "Error during installation. Is 'code' command in PATH?" && exit 1)
	@echo "$(VSIX_FILE) installed successfully."

# Default target points to build
default: build

# Target to clean generated files
clean:
	@echo "Cleaning up..."
	@rm -f $(VSIX_FILE)