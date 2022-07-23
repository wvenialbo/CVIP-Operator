# Installation

## Install and use

The library neither provides pre-built binaries nor it provides an installation
package. To use the library, you can:

1. Include the source code directly in your project.  
   This is a very small library, so you could clone the source code, add the
   `include`{.file} folder inside CVIP-Operator to your projectʼs include
   directory, copy the content of the `src`{.file} folder inside CVIP-Operator
   into your projectʼs source code folder, and use the library.
2. Build the library with the provided scripts.  
   A Visual Studio solution and project file is provided, *see below*. A CMake
   script is being cooked.

## Build the library

1. Build using Visual Studio.  
   Open the Visual Studio solution file `CVIP-Operator.sln`{.file} or the Visual
   Studio project file `vcproject/cvip.vcxproj`{.file} and build. Note that
   building from the solution file will also build the unit test project.
2. Install using CMake.  
   In the project root folder, run the commands:

        :::bash
        mkdir build
        cd build
        cmake ..

3. Build the source code:
    1. On Windows, open the `CVIP-Operator.sln`{.file} solution file and build
       with Visual Studio, or run:

            :::bash
            cmake --build . --config Release

    2. On Linux, run the `:::bash make` command:

            :::bash
            sudo make install

## Build the tests

Open the Visual Studio solution file `CVIP-Operator.sln`{.file} or the Visual
Studio project file `vcproject/cvip-test.vcxproj`{.file} and build.

There are two parts of code to test the library:

* Unit tests: They require C++11, Google Test and Google Mock.
* Tutorials: Sample code to demonstrate how to use the library. They require
  C++11. If you want to have a quick study on how to use the library, you can
  look at the tutorials.

## Build the documentation

Open a terminal shell or command console window, if you do not have a working
copy of [MkDocs](https://www.mkdocs.org/), first install one using
`:::bash pip install mkdocs`.
Also  install the following extensions:

    :::bash
    pip install mkdocs-material
    pip install pygments
    pip install pymdown-extensions

Go, `:::bash cd`, to the directory where you cloned the project and run
`:::bash mkdocs build` to build a local instance of this documentation.
