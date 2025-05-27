# folders-lock

folders-lock is a command-line utility written in C++ that creates a specified directory structure. It generates a root directory and then recursively creates 10 subdirectories (named "0" through "9") within each created directory, up to a specified depth.

## Features

*   Creates a nested directory structure based on a root name and depth.
*   Accepts the root directory as a name (created in the current path) or an absolute/relative path.
*   Optional depth parameter (defaults to 4 if not specified).
*   Provides a progress bar during directory creation.
*   Cross-platform build using CMake.
*   Standard command-line usage messages and error handling.

## Prerequisites

*   A C++17 compatible compiler (e.g., GCC, Clang, MSVC)
*   CMake (version 3.10 or higher recommended)
*   Git (for cloning the repository)

## Getting Started

### 1. Clone the Repository (Optional)

If you have cloned the repository, navigate to its root directory.
```bash
git clone https://github.com/ussdeveloper/foldersLock.git
cd foldersLock
```

### 2. Build Instructions

This project uses CMake to generate build files for your specific platform and compiler.

**General CMake Build Process:**

1.  **Create a build directory:**
    It's good practice to create a separate directory for building (e.g., `build`) to keep the source directory clean.

    ```bash
    mkdir build
    cd build
    ```

2.  **Configure the project (Generate build files):**
    Run CMake from the build directory, pointing it to the source directory (which contains `CMakeLists.txt`).

    ```bash
    cmake ..
    ```
    *   For specific generators (e.g., Visual Studio, MinGW Makefiles), you can use the `-G` option:
        *   `cmake .. -G "Visual Studio 17 2022"` (for Visual Studio 2022)
        *   `cmake .. -G "MinGW Makefiles"` (for MinGW)

3.  **Build the project:**
    After CMake has generated the build files, compile the project.

    *   For single-configuration generators (like Makefiles):
        ```bash
        cmake --build .
        ```
    *   For multi-configuration generators (like Visual Studio), specify the configuration (e.g., Release, Debug):
        ```bash
        cmake --build . --config Release
        # or
        # cmake --build . --config Debug
        ```

    The executable `folders-lock` (or `folders-lock.exe` on Windows) will be created in the build directory, typically within a subfolder like `Release` or `Debug` for multi-config generators, or directly in the build directory for single-config ones. For example, `build/Release/folders-lock.exe` or `build/folders-lock`.

### 3. Running the Program

Navigate to the directory containing the `folders-lock` executable.

**Usage:**

```
folders-lock.exe <root_directory_name_or_path> [depth]
```

**Arguments:**

*   `<root_directory_name_or_path>`: (Mandatory) The name of or path to the root directory to be created.
    *   If a name is provided (e.g., `myFolders`), it will be created in the current working directory.
    *   If a path is provided (e.g., `C:\\temp\\myFolders` or `./data/myFolders`), it will be created at that location.
*   `[depth]`: (Optional) The number of subdirectory levels to create. Defaults to 4 if not specified. Must be a positive integer.

**Examples:**

*   Create a directory named `output_data` in the current location with a default depth of 4:
    ```bash
    ./folders-lock output_data
    ```
    (On Windows, you might run `.\\Release\\folders-lock.exe output_data` from the `build` directory)

*   Create a directory structure at `C:\projects\test_structure` with a depth of 3:
    ```bash
    ./folders-lock C:\\projects\\test_structure 3
    ```

*   Create a directory structure in a relative path `../generated_dirs` with a depth of 2:
    ```bash
    ./folders-lock ../generated_dirs 2
    ```

## How it Works

The program takes the root directory name/path and an optional depth.
1.  It first creates the root directory if it doesn't exist.
2.  Then, for each level of depth:
    *   It creates 10 subdirectories named "0", "1", ..., "9".
    *   It recursively does the same for each of these newly created subdirectories until the target depth is reached.

For example, with `folders-lock my_root 2`:

```
my_root/
├── 0/
│   ├── 0/
│   ├── 1/
│   ...
│   └── 9/
├── 1/
│   ├── 0/
│   ...
│   └── 9/
...
└── 9/
    ├── 0/
    ...
    └── 9/
```

A progress bar will indicate the creation process, showing the target depth and the percentage completion.
