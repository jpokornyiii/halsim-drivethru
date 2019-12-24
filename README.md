# halsim-drivethru
WPILib HALSim Extension for Drivethru

## Introduction
`halsim_drivethru` is a HAL Extension for WPILib based robot code that will allow them to communicate with a [Drivethru](https://github.com/bb-frc-workshops/drivethru) equipped robot.

## Usage
In your WPILib robot project, open up `build.gradle`, and enter the following line into the `dependencies` block:

`simulation "com.bbfrc:halsim_drivethru:1.0:${wpi.platforms.desktop}@zip`

NOTE: The `1.0` is currently hardcoded and will be subject to change once we get versioning working.

This adds `halsim_drivethru` as a dependency to your robot project. Now, when you hit `F5` (or `Simulate Robot Code on Desktop`), it will automatically download and use the extension in simulation mode. In VS Code, it will also prompt you to select a shared library file. Simply select the `halsim_drivethru.dll` file on Windows. (TBD: Mac and Linux builds coming soon).

## Building halsim-drivethru
This project depends on a few tools, including WPILib's NativeUtils.

### Requirements
- A C++ Compiler
    - On Linux, GCC is fine
    - On Windows, you will need Visual Studio 2019 (Community Edition). Make sure to select the C++ Programming Language support when installing
- protobuf library
    - Unix-like systems
        - Follow the instructions here: https://github.com/protocolbuffers/protobuf/blob/master/src/README.md
    - Windows
        - Install [vcpkg](https://github.com/microsoft/vcpkg) following the instructions on the GitHub repo
        - Set up an environment variable named `VCPKG_ROOT` and have it point to where you downloaded vcpkg
        - Run `vcpkg.exe install protobuf:x86-windows-static protobuf:x64-windows-static`

### Setup
Clone the halsim-drivethru project. If the toolchains are not installed, install them and make sure they are available on the system PATH.

### Modifying Code
Code location follows the gradle standard, where source files (`.cpp`) are in the `src/main/cpp` directory, and header files are in the `src/main/include` directory.

#### Note about asio
We will be using the [asio C++ library](https://think-async.com/Asio/) for networking support. The library is included as part of this repository and the build file is already set up to use it. In any file that uses the asio library, please make sure you include `#define ASIO_STANDALONE` before including an asio header file. If you don't, the build will fail with the compiler complaining about not being able to find the boost library.

### Building
All build steps are executed using the Gradle wrapper, `gradlew`. To build the project, open a console and navigate to the halsim-drivethru directory. Then:

`./gradlew build`

### Publishing
The extension will need to be published before it can be consumed by robot programs.

NOTE: We have not set up publishing to Maven Central yet as this is dependent on the versioning system working properly. TODO: Fix this

To publish to your local maven repository (commonly located at `C:\Users<Username>\.m2` on Windows or `~/.m2` on *nix systems), run the following:

`./gradlew publishToMavenLocal`

This will publish the current built version to a Maven Group `com.bbfrc` with Artifact ID `halsim_drivethru`.
