# Examples of use

## Preliminaries

For creating an image operator, you define a class that implements the `:::cpp
i_operator_predicate` interface defined in `cvip/i_operator.hpp`{.file}, you
donʼt even need to derive your class from it, just declare the override[^1] for
the `:::cpp i_operator_predicate::do_apply()` member function and, eventually,
instrument your operation logic inside it or, in case of a complex process, use
`:::cpp do_apply()` as the entry point to your algorithm.

[^1]: Actually, you do not override it since it is a pure virtual member
    function.

!!! hint "Note"

    In the operator predicate you created, you may encapsulate any existing
    OpenCV method, as shown in the examples below, or implement your own
    image processing algorithm.

If your algorithm does require initialisation parameters, you will also define
the constructors taking the arguments to initialise your predicate object and,
if your operator needs the ability to be reconfigured later, it should provide a
method overriding the `:::cpp i_operator_predicate::reset()` member function.

It is not necessary to define an operator class implementing the `:::cpp
i_operator` interface, defined in `cvip/i_operator.hpp`{.file}, unless you want
to extend the basic functionalities already existing. The library provides the
`:::cpp basic_operator<*>` class template defined in `cvip/operator.hpp`{.file},
that takes a predicate type name `:::cpp T` to instance an operator template
class `:::cpp basic_operator<T>` implementing the algorithm defined in `:::cpp
T::do_apply()`.

To extend the functionality of operator objects, you either implement `:::cpp
i_operator` from scratch or derive your class or class template from `:::cpp
basic_operator<*>` or its base, `:::cpp base_operator<*>`, also defined in
`cvip/i_operator.hpp`{.file}. Refer to the API documentation for details on
those interfaces and class templates.

## Creating an image transform operator

To illustrate this, letʼs define an image colour/intensity inverter operator.

First, we define our predicate class:

    :::cpp
    #include <cvip/core/i_operator.hpp>

    class invert_predicate /* : public cvip::core::i_operator_predicate */
    {
    public:

        void do_apply(matrix& dst, matrix& src, bool const first [[maybe_unused]]);

    };

Then we define the `:::cpp invert_predicate::do_apply` method:

    :::cpp
    #include <opencv2/core.hpp>
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

And thatʼs all.

You may have realised that, at termination of `:::cpp invert_predicate::do_apply()`,
I assigned an empty matrix to the `:::cpp src` input variable. This is a
requirement in my projects because in the call to the next operator in a chain
of operators, that value will be passed by reference as the `:::cpp dst`
argument. I designed it this way because, when I am working with a set of
operators that process compatible images, that is, those with the same depth,
size, and number of channels, the value in `:::cpp dst` will be used as the
destination or temporary buffer if it has the appropriate size.

!!! info "Rationale"

    The rationale behind this is that, commonly, an algorithm requires a
    temporary buffer to perform its operation, so, in my predicates, if the
    buffer received in `:::cpp dst` is of the required size or can hold my
    temporary data, it allows me to avoid allocating/deallocating memory for
    each operation in my pipeline. Since I have not allocated any temporary
    for this case, I must clear `:::cpp src` to avoid the next operator
    overwrite the input image.

## Creating a parametric transform operator

Most of the times, an operation needs to be configured through parameters.
`:::cpp basic_operator<T>` supports forwarding of its variadic arguments. Just
define your operator predicate classes with the required overloaded
constructors. Although not an operator, *sensu stricto*, let define a predicate
to pad an image with pixels. This kind of operation is commonly needed before
applying convolutional or morphological structuring operators to an image.

Again, we first define our predicate class:

    :::cpp
    #include <cvip/core/i_operator.hpp>

    class padding_predicate /* : public cvip::core::i_operator_predicate */
    {
    public:

        padding_predicate(int const t, int const r, int const b, int const l,
                          mscalar const v = mscalar::all(0)) noexcept;

        void reset(int const t, int const r, int const b, int const l) noexcept;

        void reset(mscalar const v) noexcept;

        void do_apply(matrix& dst, matrix& src, bool const first [[maybe_unused]]);

    private:

        static auto constexpr T = 0;
        static auto constexpr R = 1;
        static auto constexpr B = 2;
        static auto constexpr L = 3;

        int m_border[4] = { 0 };

        mscalar m_value = mscalar{ mscalar::all(0) };

    };

Note that I defined one constructor taking all the required parameters and two
overloads of the `:::cpp padding_predicate::reset()` method, one for updating
the number of pixels to expand each side of the image, and other to update the
value to be assigned to the added pixels. The `:::cpp m_border` member array
stores the padding size for the top, right, bottom, and left sides. The
`m_value` member stores the pixel value to initialize the padding pixels.

    :::cpp
    padding_predicate::padding_predicate(int const t, int const r, int const b, int const l,
                                         mscalar const v) noexcept :
        m_border{ t, r, b, l },
        m_value{ v }
    {
        assert(t >= 0 and r >= 0 and b >= 0 and l >= 0);
    }

The reset member allows us to update the current parameter values of an operator
predicate. It is called from the `:::cpp basic_operator<T>::operator()` member
function.

    :::cpp
    #include <cassert>

    void padding_predicate::reset(int const t, int const r, int const b, int const l) noexcept
    {
        assert(t >= 0 and r >= 0 and b >= 0 and l >= 0);
        m_border[T] = t, m_border[R] = r, m_border[B] = b, m_border[L] = l;
    }

    void padding_predicate::reset(mscalar const v) noexcept
    {
        m_value = v;
    }

Finally, the `:::cpp padding_predicate::do_apply()` just calls the native
implementation provided by OpenCV.

    :::cpp
    #include <opencv2/core.hpp>
    #include <cassert>

    void padding_predicate::do_apply(matrix& dst, matrix& src, bool const first [[maybe_unused]])
    {
        assert(not src.empty());

        cv::copyMakeBorder(src, dst, m_border[T], m_border[B], m_border[L],
                           m_border[R], cv::BORDER_CONSTANT, m_value);

        src = matrix{ };
    }

## What have we gained?

So, what have we gained by creating all these predicates and operators? Here is
an example of code that will load an image, invert it and pad it with two black
pixels on each side:

    ::cpp
    #include <cvip/core/operator.hpp>

    // Alias your operators

    using Inv = cvip::core::basic_operator<invert_predicate>;
    using Pad = cvip::core::basic_operator<padding_predicate>;

    // Load your image

    auto im1  = cv::imread("my_dataset/lena.tif", cv::IMREAD_UNCHANGED);

    // Instance and configure your operators

    auto P    = Pad{ 2, 2, 2, 2 };

    // Apply your pipeline

    auto im2  = P * Inv{} * im1;

In the last line, the invert operator is applied as a temporary rvalue. The
equivalent traditional OpenCV code is less elegant:

    ::cpp
    #include <opencv2/core.hpp>

    // Load your image

    auto im1  = cv::imread("my_dataset/lena.tif", cv::IMREAD_UNCHANGED);

    // Apply your pipeline

    auto tmp = cv::Mat{ };

    if (CV_8U <= im1.depth() and im1.depth() <= CV_32S)
    {
        cv::bitwise_not(im1, tmp);
    }
    else if (CV_32F <= im1.depth() and im1.depth() <= CV_16F)
    {
        cv::subtract(mscalar::all(1.0), im1, tmp);
    }

    auto im2 = cv::Mat{ };

    cv::copyMakeBorder(tmp, im2, 2, 2, 2, 2, cv::BORDER_CONSTANT, cv::Scalar::all(0));

Moreover, the program flux is obscure, it is not that evident from reading the
code what action is executing in each line.

For updating the parameters of an operator for reusing, you invoke the `:::cpp
operator()` method on the operator object, with the appropriate data type. The
current implementation will forward those parameters to your predicateʼs `:::cpp
reset()` member function accordingly:

    ::cpp
    // Update the padding amount on each side

    P(5, 5, 5, 5);

    // Update the padding pixels value

    P(cv::Scalar::all(255));

    // Reuse your operator

    auto z = P * y;

As we do above with the `:::cpp Inv` operator, for a once time use you can use
an rvalue:

    ::cpp
    auto y = Pad{ 2, 2, 2, 2 } * x;

I recommend not to abuse with such notation since it will obscure your code,
reducing the clarity we are pursuing.

The same if you are updating the parameters of an existing operator object, the
grammar allows:

    ::cpp
    // Change the padding amount but preserves the new pixel values

    auto y = P(5, 5, 5, 5) * x;

    // Change tha new pixels value while preserving the padding amount

    auto z = P(cv::Scalar::all(255)) * x;

But beware that **these operations cannot be concatenated!** The following code
apply `:::cpp P` to `:::cpp x` twice with different parameter values:

    ::cpp
    auto y = P(cv::Scalar::all(255)) * P(5, 5, 5, 5) * x;

probably not what you want. In such a case you can concatenate the updating
calls, like this

    ::cpp
    auto y = P(5, 5, 5, 5)(cv::Scalar::all(255)) * x;
    // or indistinctly, unless the order is relevant, not in this case
    auto z = P(cv::Scalar::all(255))(5, 5, 5, 5) * x;

However, **I am completely against this usage**, it is just a side effect of the
grammar design.

### What have we gained? (wrap up)

Weʼve gained simplicity while preserving efficiency, expressions and operators
are lightweight opaque proxy objects and predicates are lightweight objects as
well, the overhead added while constructing a pipeline expression is negligible.
