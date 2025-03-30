#!/bin/bash

# Check if vsce is installed
if ! command -v vsce &> /dev/null
then
    echo "Error: 'vsce' command not found."
    echo "Please install it globally using: npm install -g vsce"
    exit 1
fi

echo "Packaging Kimbie Dark ++ theme..."
vsce package

# Check the exit code of vsce package
if [ $? -eq 0 ]; then
    echo "Packaging complete. Look for the .vsix file in the current directory."
else
    echo "Error during packaging."
    exit 1
fi