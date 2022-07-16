# CVIP-Operator #

CVIP-Operator is a C++ library for enabling generic operator semantics when
working with OpenCV matrices/images.

## Contents ##

* [Quick summary](#quick-summary)
* [Examples of use](#examples-of-use)
* [Testing](#testing)
* [Acknowledgements](#acknowledgements)
* [Legal stuff](#legal-stuff)


## Quick summary ##

The library is written in ISO-compliant C++ to facilitate efficiency,
portability, and accessibility from other programming languages.
Internally references the OpenCV library for handling matrices/images.
The current version is supported under BSD, Linux and Windows platforms.

These header files are part of
[The CVIP Project](https://github.com/wvenialbo/CVIP), a larger library in
development, written mainly for academic purposes but also handy for general
use.[^Remark]

[^Remark]: This is a slice of the main repository, so, it does not contain
          all the commit history; however, it is keep syncronised with the
          main project.


### What this toolset does? ###

The small and light weight, almost header-only, library, add interfaces to
provide generic operator semantics, that is, given a user defined matrix/image
operation predicate, and a matrix, x, compatible with that operation, being
`P` an operator that encapsulates or implement the predicate, you can write
your code like:

```cpp
  y = P * x;
```

This allows us to write an image processing pipeline with a compact notation,
friendly to the mathematically inclined, by concatenating operators. For
instance:

```cpp
  // Instatiate and initialize your n operators

  my_operator1 P1{ ... };
  my_operator2 P2{ ... };

  ...

  my_operatorN Pn{ ... };

  // Perform any extra required configuration

  ...

  // Apply your pipeline

  auto x = cv::imread("path/to/my/source/image.png", cv::IMREAD_UNCHANGED);

  auto y = Pn * ... * P2 * P1 * x;
```

Operators and matrices are evaluated from right to left, although operations
among operators are evaluated, from left to right. Operators are, in general,
non-commutative. To achieve right-to-left evaluation operators are gathered
into an expression object before they are applied, in tandem, to the input
matrix.


## Examples of use ##

For creating a matrix/image operator, you just create a class implementing the
`i_operator_predicate` interface defined in `core/i_operator.hpp`, you even do
not need to derive your class from it. In the `do_apply` member function you
implement your operation logic or, if it is a complex algorithm, use that
member as the entry point to your algorithm.

> Note: You do not need to implement the `i_operator` nor derive a class from
  `base_operator<T>` or `basic_operator<T>` template classes, unless you want
  to extend the operator functionality.

In the operator predicate you just created, you may encapsulate any existing
OpenCV method, or implement your operation algorithm.

To illustrate this, let's define an image color/intensity inverter operator.

First, we define our predicate class:

```cpp
#include <cvip/core/i_operator.hpp>

class invert_predicate /* : public cvip::core::i_operator_predicate */
{
public:

    void do_apply(matrix& dst, matrix& src, bool const first [[maybe_unused]]);

};
```

Then we define the `invert_predicate::do_apply` method:

```cpp
#include <cassert>

void invert_predicate::do_apply(matrix& dst, matrix& src, bool const first [[maybe_unused]])
{
    assert(not src.empty());

    if (CV_8U <= src.depth() and src.depth() <= CV_32S)
    {
        cv::bitwise_not(src, dst);
    }
    else if (CV_32F <= src.depth() and src.depth() <= CV_16F)
    {
        cv::subtract(mscalar::all(1.0), src, dst);
    }

    src = matrix{ };
}
```
And that's all. Now you use it this way:

```cpp
#include<cvip/core/operator.hpp>

  using Inv = cvip::core::basic_operator<invert_predicate>;

  auto im1 = cv::imread("my_dataset/lenna.tif", cv::IMREAD_UNCHANGED);
  auto im2 = cv::imread("my_dataset/cameraman.tif", cv::IMREAD_UNCHANGED);

  // Create an instance for multiple uses

  auto P = Inv{ };

  // Apply operator

  auto im3 = P * im1;
  auto im4 = P * im2;
  
  // Or just apply it on-the-fly for one-time-use
  
  auto im5 = Inv{} * im1;
```

You may have realised that, at termination of `invert_predicate::do_apply`,
I assigned an empty matrix to the `src` input variable. This is a requirement
in my projects because in the call to the next operator in a chain of
operators, that value will be passed by reference as the `dst` argument. I
designed it this way because, when I am working with a set of operators that
process compatible images, that is, those with the same depth, size, and number
of channels, the value in `dst` will be used as the destination or temporary
buffer if it has the appropriate size.
  
> Note: The rationale behind this is that, commonly, an algorithm requires a
  temporary buffer to perform its operation, so, in my predicates, if the
  buffer received in dst is of the required size or can hold my temporary
  data, it allows me to avoid allocating/deallocating memory for each operation
  in my pipeline. Since I have not allocated any temporary for this case,
  I must clear src to avoid the next operator overwrite the input image.


### Observation

You my find that I aliased the OpenCV matrix class `cv::Mat` as `cvip::matrix`
in `config/basic_types.hpp` and imported `cv::swap` into the `::cvip` namespace
in `config/basic_imports.hpp`. That's because I use to map this toolset to other
matrix libraries. That is, the source code in this library is OpenCV agnostic,
you can map to any other matrix library just replacing the above aliases. Even
more, you can replace the matrix type alias with any other type for which you
need the semantics provided by this toolset.


## Testing ##

The source code was unit tested with Google Test/Mock. Compiler compatibility was
proven with

- Clang C++ compiler v. 7.0/8.0,
- GNU C++ compiler v. 4.3/7.1/8.3/9.3,
- Microsoft Visual C++ 2019 v. 16.11,

for C++ and C++11/14/17/20.


## To dos ##

- [ ] Create and add documentation.


## Acknowledgements ##

The following online tools were used to validate compiler and language compatibility:

- https://ideone.com/1IZ2RN
  * gcc 4.3 (C++)
  * gcc 8.3 (C++/C++14)
  * clang 8.0 (C++14)
- https://www.programiz.com/cpp-programming/online-compiler/
  * gcc 8.3 (C++14)
- https://www.onlinegdb.com/online_c++_compiler
  * gcc 9.4 (C++/C++14/C++17/C++20†)  
    † C++17 is reported by the C++20 option
- https://rextester.com/l/cpp_online_compiler_gcc
  * gcc 7.4 C++14
  * clang 6.0 C++14
- https://www.tutorialspoint.com/compile_cpp11_online.php
  * gcc 7.1 C++11
- https://techiedelight.com/compiler/
  * clang 7.0 C++14
  * gcc 7.4 C++17
  * gcc 8.3 C++17
  * gcc 9.2 C++17


### Remarks ###

The source code file headers mention the name ‘AsterAID Project’, that is
because the code was originally written for an asteroid detection software
package called that way. Speaking-of, you can follow this amazing
[space rock](https://ssd.jpl.nasa.gov/tools/sbdb_lookup.html#/?sstr=16696&view=VOP)
as it wanders the Solar System. Also,
[here](http://www.minorplanetcenter.net/db_search/show_object?object_id=16696).


## Legal stuff ##

### License ###

This project is licensed under combination of **BSD-3-Clause-Clear** and
**BSD-3-Clause-Attribution** licenses identified by the string
`BSD-3-Clause-Clear+`. See LICENSE file in the project root for full license
information.

The [*BSD 3-Clause Clear License*](https://spdx.org/licenses/BSD-3-Clause-Clear.html)
(`BSD-3-Clause-Clear`) is same as BSD-3-Clause but with explicit statement
as to no patent rights granted in last paragraph.

The [*BSD with attribution* license](https://spdx.org/licenses/BSD-3-Clause-Attribution.html)
(`BSD-3-Clause-Attribution`) is identical to BSD-3-Clause, with the exception
of clause 4. Clause 4 adds an “attribution” requirement, not to be confused
with the “advertising” clause found in old (original) BSD-4-Clause license
texts. It is Free, but GPL compatibility is unclear at this time.


### Copyright notice ###

© 2019-2022, Waldemar Villamayor-Venialbo. All Rights Reserved.

**OpenCV**, **Google Test**, **Google Mock**, **Clang C++**, **GNU C++**,
**Microsoft Visual C++**, **AsterAID** and **Masturbot** are trademarks
of their copyright owners. Other trademarks may be the property of their
respective owners.

This README file is part of The CVIP Project. The content of this source
code is licensed under a version of the BSD License as stated below. See
[LICENSE](LICENSE.md) file in the project root for full license
information.
