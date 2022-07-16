#include <gmock/gmock.h>
#include <cvip/config/basic_imports.hpp>
#include <cvip/core/expression.hpp>
#include <cvip/core/operator.hpp>


using cvip::matrix;


struct my_operator1 : public cvip::core::base_operator< my_operator1 >
{
    my_operator1() = default;

    my_operator1(my_operator1 const&) { }

    using matrix = cvip::matrix;

    MOCK_METHOD(virtual void, apply, (matrix& dst, matrix& src, bool const first), (override));
};


TEST(OperatorExpression, OperatorApplyGetCalledOnce)
{
    using ::testing::_;

    auto op = my_operator1{ };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op, apply(_, _, _)).Times(1);

    auto y = op * x;
}


TEST(OperatorExpression, OperatorApplyGetCalledAsFirstInChain)
{
    using ::testing::_;

    auto op = my_operator1{ };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op, apply(_, _, true)).Times(1);

    auto y = op * x;
}


// In an expression, an operator object is cloned and we can't setup
// expectations on the object itself, so create an operator that store
// a reference to the first concrete object in the cloning chain and
// forward the call to an auxiliary mock method.
//

struct my_operator2 : public cvip::core::base_operator< my_operator2 >
{
    my_operator2()
    {
        p_first = this;
    }

    my_operator2(my_operator2 const& src)
    {
        p_first = src.p_first;
    }

    using matrix = cvip::matrix;

    MOCK_METHOD(void, do_apply, (size_t dst, size_t src, bool const first), ());

    virtual void apply(matrix& dst, matrix& src, bool const first) override
    {
        // We also test the objects are swapped correctly and the appropriate
        // buffer is received in each call
        //
        // The unit test method creates a 2x2 matrix, we expect this matrix is
        // received as the src in each call. In the first call dst should be an
        // empty matrix, so, dst.total() == 0; we allocate a 3x3 matrix into src
        // after swapping in with dst. In the second call we expect src to be a
        // 2x2 matrix, a clone of the original source, and dst to be the 3x3
        // matrix allocated in the previous invocation.
        //
        p_first->do_apply(dst.total(), src.total(), first);
        cvip::swap(dst, src);
        if (first)
        {
            src = cvip::matrix(3, 3, CV_8UC1);
        }
    }

    my_operator2* p_first;
};


TEST(OperatorExpression, OperatorApplyGetCalledInProperOrder)
{
    using ::testing::_;

    auto op1 = my_operator2{ };
    auto op2 = my_operator2{ };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op1, do_apply(_, _, true)).Times(1);
    EXPECT_CALL(op2, do_apply(_, _, false)).Times(1);

    auto y = op2 * op1 * x;
}


TEST(OperatorExpression, OperatorApplyGetCalledWithProperValues)
{
    auto op1 = my_operator2{ };
    auto op2 = my_operator2{ };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    // We also test the objects are swapped correctly and the appropriate
    // buffer is received in each call
    //
    // The unit test method creates a 2x2 matrix, we expect this matrix is
    // received as the src in each call. In the first call dst should be an
    // empty matrix, so, dst.total() == 0; we allocate a 3x3 matrix into src
    // after swapping in with dst. In the second call we expect src to be a
    // 2x2 matrix, a clone of the original source, and dst to be the 3x3
    // matrix allocated in the previous invocation.
    //
    EXPECT_CALL(op1, do_apply(0, 4, true)).Times(1);
    EXPECT_CALL(op2, do_apply(9, 4, false)).Times(1);

    auto y = op2 * op1 * x;
}


// The operator is in charge of instatiating the predicate and it also gets
// cloned each single time the operator clones itself. Therefore we should
// use the same stragegy as before besides mocking on the data member the
// operator allocates for the predicate.
//


struct my_predicate1 /* : public cvip::core::i_operator_predicate */
{
    my_predicate1() = default;

    my_predicate1(my_predicate1& mocking)
    {
        p_first = &mocking;
    }

    my_predicate1(my_predicate1 const& src)
    {
        p_first = src.p_first;
    }

    using matrix = cvip::matrix;

    MOCK_METHOD(void, do_apply2, (size_t dst, size_t src, bool const first), ());

    void do_apply(matrix& dst, matrix& src, bool const first)
    {
        p_first->do_apply2(dst.total(), src.total(), first);
        cvip::swap(dst, src);
        if (first)
        {
            src = cvip::matrix(3, 3, CV_8UC1);
        }
    }

    my_predicate1* p_first;
};


TEST(OperatorExpression, PredicateDoApplyGetCalledInProperOrder)
{
    using ::testing::_;

    using my_op = cvip::core::basic_operator<my_predicate1>;

    auto mocking = my_predicate1{ };

    auto op1 = my_op{ mocking };
    auto op2 = my_op{ mocking };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(mocking, do_apply2(_, _, true)).Times(1);
    EXPECT_CALL(mocking, do_apply2(_, _, false)).Times(1);

    auto y = op2 * op1 * x;
}


TEST(OperatorExpression, PredicateDoApplyGetCalledWithProperValues)
{
    using my_op = cvip::core::basic_operator<my_predicate1>;

    auto mocking = my_predicate1{ };

    auto op1 = my_op{ mocking };
    auto op2 = my_op{ mocking };

    auto x = cvip::matrix(2, 2, CV_8UC1);

    // We also test the objects are swapped correctly and the appropriate
    // buffer is received in each call
    //
    // The unit test method creates a 2x2 matrix, we expect this matrix is
    // received as the src in each call. In the first call dst should be an
    // empty matrix, so, dst.total() == 0; we allocate a 3x3 matrix into src
    // after swapping in with dst. In the second call we expect src to be a
    // 2x2 matrix, a clone of the original source, and dst to be the 3x3
    // matrix allocated in the previous invocation.
    //
    EXPECT_CALL(mocking, do_apply2(0, 4, true)).Times(1);
    EXPECT_CALL(mocking, do_apply2(9, 4, false)).Times(1);

    auto y = op2 * op1 * x;
}
