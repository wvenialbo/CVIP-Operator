# CVIP-Operator

![C++11](https://img.shields.io/badge/C++-11-informational)
![Compiler: Clang / GCC / MSVC](https://img.shields.io/badge/compilers-Clang%20%2F%20GCC%20%2F%20MSVC-informational)
![Platform: BSD | Linux | Win-64](https://img.shields.io/badge/platform-BSD--64%20%7C%20Linux--64%20%7C%20Win--64-informational)
![License: BSD-like](https://img.shields.io/badge/license-BSD--like-informational)  
![Build: passing](https://img.shields.io/badge/build-passing-success)
![Test: passing](https://img.shields.io/badge/tests-8%20passed%2C%200%20failed%2C%200%20skipped-success)
![Coverage: 100%](https://img.shields.io/badge/coverage-100%25-success)
![Docs: partial](https://img.shields.io/badge/docs-partial-yellow)

## About {.about}

CVIP-Operator is a C++ library for enabling generic operator semantics when
working with OpenCV matrices/images.

### Topics {.topics}

[**`cpp`**](https://github.com/topics/cpp){.topics}
[**`opencv`**](https://github.com/topics/opencv){.topics}
[**`computer-vision`**](https://github.com/topics/computer-vision){.topics}
[**`image-processing`**](https://github.com/topics/image-processing){.topics}

## Contents {.hidden-toc}

- [Introduction](#introduction)
- [Overview](overview.md)
- [Requirements and testing](requirements.md)
- [Installation](installation.md)
- [Examples of use](examples.md)
- [Releases & Change log](releases.md)
- [Acknowledgements](acknowledgements.md)
- [License & legal stuff](legal.md)

## Introduction

CVIP-Operator is part of the [The CVIP++ Project][cvip-project], an open-source
library written in C++ under the terms of a [BSD-like License](legal.md) for use
in computer vision or image processing projects. It is a compilation of software
written by me mainly for academic or research purposes, or for entertainment. 😁

The original source code dates from early 1990ʼs to mid 2010ʼs, now retargeted
to Modern C++ and OpenCV, taking advantage of the amazing new features of the
latest C++ language standards and the blasting performance of OpenCV matrix data
handling; the original software was linked to an old matrix/image handling
library written by me, but here I redeeming some features that I miss when
writing programs with OpenCV.

I am including (reimplementing) in CVIP++ some algorithms that OpenCV lacks,
like thinning (skeletonization) algorithms, or some alternative algorithms to
those existing in OpenCV; in such cases, I also map the OpenCV native
implementation using the semantics of CVIP++.

When you work with vectors and array of vectors in the form of matrices, linear
operators are matrices and operands are other vectors or matrices, and you
operate on them with multiplication; or additions. Image operations arenʼt
linear, strictly speaking, and images are not array of vectors, *stricto sensu*,
but representing an image processing pipeline with operator notation is
practical and convenient, and you feel comfortable with it when coming from a
mathematical background.

So, go ahead to the [CVIP-Operator Overview](overview.md) section.

[cvip-project]: https://github.com/wvenialbo/CVIP "The CVIP++ Project"
