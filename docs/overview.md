# Overview

## Quick summary

The library is written in ISO-compliant C++ to facilitate efficiency,
portability, and accessibility from other programming languages. Internally
references the [OpenCV][opencv] library for handling matrices or images. The
current version is supported under BSD, Linux and Windows platforms.

These header files are part of [The CVIP++ Project][cvip-project], a larger
library in development, written mainly for academic purposes but also handy for
general use.[^1]

!!! info "Contributing"

    Contributions to this project are welcome and encouraged.

[^1]: This is a slice of the main repository, so, it does not contain all the
      commit history; however, it is kept synchronised with the main projectʼs
      repository.

## What this toolset does?

The small and light weight library, add interfaces to provide generic operator
semantics to image processing pipelines, that is, given a user defined image
processing algorithm `A`{.none}, and a matrix, `x`{.none}, compatible with that
operation, being `P ⟵ P<A>`{.none} an operator that encapsulates that algorithm
`A`{.none}, you can write your code like: `y = P * x`{.none}.

This allows us to write an image processing pipeline with a compact notation,
friendly to the mathematically inclined, by concatenating operators. For
instance, something like this:

    ::cpp
    auto x  = cv::imread("my_dataset/cameraman.tif", cv::IMREAD_UNCHANGED);

    auto tmp = cv::Mat{ };

    if (CV_8U <= x.depth() and x.depth() <= CV_32S)
    {
        cv::bitwise_not(x, tmp);
    }
    else if (CV_32F <= x.depth() and x.depth() <= CV_16F)
    {
        cv::subtract(mscalar::all(1.0), x, tmp);
    }

    auto y = cv::Mat{ };

    cv::copyMakeBorder(tmp, y, 2, 2, 2, 2, cv::BORDER_CONSTANT, cv::Scalar::all(0));

can be written more compactly this way:

    ::cpp
    auto y = P2 * P1 * x;

A complex pipeline can be reduced to:

    ::cpp
    auto y = Pn * ... * P2 * P1 * x;

Operators acting on matrices are evaluated from right to left, i.e. in our
previous examples, `:::cpp P2` is operating on the result of `:::cpp P1 * x`,
although operator concatenation is evaluated from left to right. Operators are,
in general, non-commutative. To achieve right-to-left evaluation, operators are
gathered into an expression object before they are applied, in tandem, to the
input matrix.

## Motivation

When you use operator semantics the code looks cleaner and intuitive, and it is
easier to maintain. You can see the whole pipeline process at once and are able
to detect any inconsistency just viewing one line of code rather than analysing
a bunch of lines with function calls and methods with somehow cryptic names.

Operator semantics, if well designed, helps you to write a solution in the
language of the problem, with the same terms you think when solving real world
problems.

## Namespace

All definitions are within the namespace `:::cpp ::cvip`, which have other
namespaces according to the topic of the contained source code. The required
classes for providing operator semantics are under the `:::cpp ::cvip::core`
namespace.

!!! note "Remark"

    You may find that I aliased the OpenCV matrix class `:::cpp cv::Mat` as
    `:::cpp cvip::matrix` in `cvip/internal/basic_types.hpp`{.file} and
    imported `:::cpp cv::swap` into the `:::cpp ::cvip` namespace in
    `cvip/internal/basic_imports.hpp`{.file}. Thatʼs because I use to map this
    toolset to other matrix libraries. That is, the source code in
    CVIP-Operator library is OpenCV agnostic, you can map to any other matrix
    library just replacing the above aliases. Even more, you can replace the
    matrix type alias with any other type for which you need the semantics
    provided by this toolset.

[cvip-project]: https://github.com/wvenialbo/CVIP "The CVIP++ Project"
[opencv]: https://opencv.org/ "OpenCV.org"
