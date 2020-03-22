# halsim-drivethru
WPILib HALSim Extension for Drivethru

## Introduction
`halsim_drivethru` is a HAL Extension for WPILib based robot code that will allow them to communicate with a [Drivethru](https://github.com/bb-frc-workshops/drivethru) equipped robot.

## Usage
In your WPILib robot project, open up `build.gradle`, and enter the following line into the `dependencies` block:

`simulation "com.bbfrc:halsim_drivethru:1.0:${wpi.platforms.desktop}@zip"`

NOTE: The `1.0` is currently hardcoded and will be subject to change once we get versioning working.

and into the `repositories` block:

`mavenLocal()`

This adds `halsim_drivethru` as a dependency to your robot project and adds your local maven repository to it's search path for the dll. Now, when you hit `F5` (or `Simulate Robot Code on Desktop`), it will automatically download and use the extension in simulation mode. In VS Code, it will also prompt you to select a shared library file. Simply select the `halsim_drivethru.dll` file on Windows. (TBD: Mac and Linux builds coming soon).

## Building halsim-drivethru
This project depends on a few tools, including WPILib's NativeUtils.

### Requirements
- A C++ Compiler
    - On Linux, GCC is fine
    - On Windows, you will need Visual Studio 2019 (Community Edition). Make sure to select the C++ Programming Language support when installing

### Setup
Clone the halsim-drivethru project. If the toolchains are not installed, install them and make sure they are available on the system PATH.

### Modifying Code
Code location follows the gradle standard, where source files (`.cpp`) are in the `src/main/cpp` directory, and header files are in the `src/main/include` directory.

#### Note about asio
We will be using the [asio C++ library](https://think-async.com/Asio/) for networking support. The library is included as part of this repository and the build file is already set up to use it. In any file that uses the asio library, please make sure you include `#define ASIO_STANDALONE` before including an asio header file. If you don't, the build will fail with the compiler complaining about not being able to find the boost library.

#### Note about FlatBuffers
We will be using [FlatBuffers](https://github.com/google/flatbuffers) as the message encoder. The library is included as part of this repository and the build file is already set up to use it.

The schema is located in the `schemas/flatbuffers` folder and requires the `flatc` compiler to generate the necessary header files. On Windows, a pre-built compiler can be downloaded from the [releases](https://github.com/google/flatbuffers/releases) page. On Linux/Unix systems, follow the instructions on the [landing page](https://google.github.io/flatbuffers/flatbuffers_guide_building.html).

Once you have `flatc` compiled or downloaded, navigate to the `schemas/flatbuffers` folder and run `flatc --cpp -o ../../src/main/include drivethru.fbs`. This will generate the appropriate `drivethru_generated.h` header file in the `include` folder.

### Building
All build steps are executed using the Gradle wrapper, `gradlew`. To build the project, open a console and navigate to the halsim-drivethru directory. Then:

`./gradlew build`

### Publishing
The extension will need to be published before it can be consumed by robot programs.

NOTE: We have not set up publishing to Maven Central yet as this is dependent on the versioning system working properly. TODO: Fix this

To publish to your local maven repository (commonly located at `C:\Users\<Username>\.m2` on Windows or `~/.m2` on *nix systems), run the following:

`./gradlew publishToMavenLocal`

This will publish the current built version to a Maven Group `com.bbfrc` with Artifact ID `halsim_drivethru`.
