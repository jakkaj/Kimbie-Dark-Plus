# VS Code Theme Packaging Plan (`.vsix`)

This document outlines the plan to create a script for packaging the Kimbie Dark ++ theme into a `.vsix` file for local installation.

## Approach

We will use a standalone shell script (`package.sh`) that utilizes the `vsce` (Visual Studio Code Extensions) command-line tool.

## Plan

1.  **Prerequisite:** Ensure `vsce` is installed globally. If not already installed, run the following command in your terminal:
    ```bash
    npm install -g vsce
    ```
2.  **Create `package.sh`:** Create a file named `package.sh` in the project root directory (`/home/jak/github/Kimbie-Dark-Plus`).
3.  **Add Script Content:** Populate `package.sh` with the following content:
    ```bash
    #!/bin/bash
    # Ensure vsce is installed globally: npm install -g vsce
    echo "Packaging Kimbie Dark ++ theme..."
    vsce package
    echo "Packaging complete. Look for the .vsix file in the current directory."
    ```
4.  **Make Executable:** Make the script executable by running the following command in the terminal from the project root:
    ```bash
    chmod +x package.sh
    ```
5.  **Execute:** Run the script from the project root directory to generate the `.vsix` package:
    ```bash
    ./package.sh
    ```
    The `.vsix` file (e.g., `kimbie-dark-plus-plus-1.3.0.vsix`) will be created in the project root directory.

## Process Flow

```mermaid
graph TD
    A[User ensures 'vsce' is installed globally] --> B{Create package.sh};
    B --> C[Add packaging commands to script];
    C --> D[User makes script executable (chmod +x package.sh)];
    D --> E[User runs ./package.sh];
    E --> F[Generates .vsix file];