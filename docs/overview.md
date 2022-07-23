# Overview

## Quick summary

The library is written in ISO-compliant C++ to facilitate efficiency,
portability, and accessibility from other programming languages. Internally
references the OpenCV library for handling matrices/images. The current version
is supported under BSD, Linux and Windows platforms.

These header files are part of [The CVIP++ Project][cvip-project], a larger
library in development, written mainly for academic purposes but also handy for
general use.[^1]

Source code: <https://github.com/wvenialbo/CVIP-Operator>

!!! info "Contributing"

    Contributions to this project are welcome and encouraged.

[^1]: This is a slice of the main repository, so, it does not contain all the
      commit history; however, it is kept synchronised with the main projectʼs
      repository.

## What this toolset does?

The small and light weight, almost header-only, library, add interfaces to
provide generic operator semantics, that is, given a user defined matrix/image
operation predicate, and a matrix, `x`{.none}, compatible with that operation,
being `P`{.none} an operator that encapsulates or implement the predicate, you
can write your code like: `y = P * x`{.none}.

This allows us to write an image processing pipeline with a compact notation,
friendly to the mathematically inclined, by concatenating operators. For
instance:

    ::cpp
    // Instatiate and initialize your n operators

    my_operator1 P1{ ... };
    my_operator2 P2{ ... };
    ...
    my_operatorN Pn{ ... };

    // Perform any extra required configuration

    ...

    // Load your image

    auto x = cv::imread("my_dataset/lena.tif", cv::IMREAD_UNCHANGED);

    // Apply your pipeline

    auto y = Pn * ... * P2 * P1 * x;

Operators and matrices are evaluated from right to left, although operations
among operators are evaluated, from left to right. Operators are, in general,
non-commutative. To achieve right-to-left evaluation operators are gathered into
an expression object before they are applied, in tandem, to the input matrix.

## Motivation

When you use operator semantics the code looks cleaner and intuitive and easier
to maintain. You can see the whole pipeline process at once and are able to
detect any inconsistency just viewing one line of code rather than analysing a
bunch of lines with function calls and methods with somehow cryptic names.

Operator semantics, if well designed, helps you to write a solution in the
language of the problem, with the same terms you think when solving real world
problems.

## Namespace

All definitions are within the namespace `:::cpp ::cvip`, which have other namespaces
according to the topic of the contained source code. The required classes for
providing operator semantics are in the `:::cpp ::cvip::core` namespace.

[cvip-project]: https://github.com/wvenialbo/CVIP "The CVIP++ Project"
