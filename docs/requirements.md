# Requirements and testing

## C++ standard requirements

The library uses C++11 features, so, any compiler that has good support for
C++11 should be able to compile the library.

## Dependencies

Programs linking against CVIP-Operator, including the testing application,
requires an installation of the [OpenCV][opencv] library and binaries. The
current code was tested with OpenCV 4.5.4 and 4.6.0.

## Testing

The source code was unit tested with the [Google Test][gtest] framework using
only C++11 features. Unit tests and test coverage measurement was carried out in
Debug mode and Release mode with the `:::cpp _DEBUG` flag defined to preserve
assertions.

## Supported compilers

Compiler compatibility was proven with:

- Clang C++ compiler v. 7.0/8.0,
- GNU C++ compiler v. 4.3/7.1/8.3/9.3,
- Microsoft Visual C++ 2019 v. 16.11,

for C++11/14/17/20. *See [Acknowledgements](acknowledgements.md)*.

## Documentation

In order to build the documentation, you require a working installation of
[MkDocs](https://www.mkdocs.org/).

[gtest]: https://github.com/google/googletest "GoogleTest - Google Testing and Mocking Framewor"
[opencv]: https://opencv.org/ "OpenCV.org"
