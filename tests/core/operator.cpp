#include <gmock/gmock.h>
#include <cvip/config/basic_imports.hpp>
#include <cvip/core/expression.hpp>
#include <cvip/core/operator.hpp>


using cvip::matrix;


// In an expression, an operator object is cloned and we can't setup
// expectations on the object itself, so create an operator that store
// a reference to the first concrete object in the cloning chain and
// forward the call to an auxiliary mock method.
//

struct basic_operator_fake : public cvip::core::base_operator< basic_operator_fake >
{
    basic_operator_fake()
    {
        p_first = this;
    }

    basic_operator_fake(basic_operator_fake const& src)
    {
        p_first = src.p_first;
    }

    using matrix = cvip::matrix;

    MOCK_METHOD(void, apply, (size_t dst, size_t src, bool const first), ());

    virtual void apply(matrix& dst, matrix& src, bool const first) override
    {
        p_first->apply(dst.total(), src.total(), first);
        cvip::swap(dst, src);
        if (first)
        {
            ASSERT_EQ(src.total(), 0);
            src = cvip::matrix(3, 3, CV_8UC1);
        }
    }

    basic_operator_fake* p_first;
};


// basic_operator<T> use a compile-time ‘dependency injection’ pattern, that
// i, the operator object is in charge of instatiating the predicate T, and the
// predicate object also gets cloned each single time the operator clones
// itself. Therefore we should use the same stragegy as before to pr the
// T::do_apply() method.
//

struct testing_predicate /* : public cvip::core::i_operator_predicate */
{
    testing_predicate() = default;

    testing_predicate(testing_predicate& pr)
    {
        p_first = &pr;
    }

    testing_predicate(testing_predicate const& src)
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
            ASSERT_EQ(src.total(), 0);
            src = cvip::matrix(3, 3, CV_8UC1);
        }
    }

    testing_predicate* p_first;
};


// The unit tests
//
// OperatorExpression::OperatorApplyGetCalledOnce
//
// and
//
// OperatorExpression::OperatorApplyGetCalledAsFirstInChain
//
// test that the overloaded operator*(basic_operator<T>, matrix) defined in
// src/cvip/core/operator.cpp forwards the call to basic_operator<T>::apply()
// defined in include/cvip/core/operator.inl.
//
// Test are performed using a fake operator object of class
// basic_operator_fake, since we cannont mock the protected method
// basic_operator<T>::apply().
//
TEST(OperatorExpression, OperatorApplyGetCalledOnce)
{
    using ::testing::_;

    auto op = basic_operator_fake{ };
    auto x  = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op, apply(_, _, _)).Times(1);

    auto y = op * x;
}


TEST(OperatorExpression, OperatorApplyGetCalledAsFirstInChain)
{
    using ::testing::_;

    auto op = basic_operator_fake{ };
    auto x  = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op, apply(_, _, true)).Times(1);

    auto y = op * x;
}


// The unit tests
//
// OperatorExpression::OperatorApplyGetCalledInProperOrder,
//
// and
//
// OperatorExpression::OperatorApplyGetCalledWithProperValues (*)
//
// test that the overloaded operator*(i_operator, i_operator) defined in
// include/cvip/core/expression.inl forwards the call to the appropriate
// methods on operator_expression defined in the same file and in
// src/cvip/core/operator.cpp, to create the list of operators that should be
// applied to a given matrix.
//
// They also test the overloaded operator*(operator_expression, i_operator)
// with the same goal and, finally, operator*(operator_expression, matrix),
// to check that the operators are applied in the proper order, from right
// to left, and with the corresponding arguments passed by the user.
//
// Test are performed using a fake operator object of class
// basic_operator_fake, since we cannont mock the protected method
// basic_operator<T>::apply().
//
// The second test, marked (*), also test the overloaded
// operator*(i_operator, operator_expression) using the right associative
// property of operator chains. It also check that the objects are swapped
// correctly and the appropriate buffer is received in each call.
//
// The unit test methods create a 2x2 matrix, we expect this matrix is
// received as the src in each call. In the first call dst should be an
// empty matrix, so, dst.total() == 0; we allocate a 3x3 matrix into src
// after swapping in with dst. In the second call we expect src to be a
// 2x2 matrix, a clone of the original source, and dst to be the 3x3
// matrix allocated in the previous invocation.
//
//

TEST(OperatorExpression, OperatorApplyGetCalledInProperOrder)
{
    using ::testing::_;

    auto op1 = basic_operator_fake{ };
    auto op2 = basic_operator_fake{ };
    auto x   = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op1, apply(_, _, true)).Times(1);
    EXPECT_CALL(op2, apply(_, _, false)).Times(2);

    auto y = op2 * op2 * op1 * x;
}


TEST(OperatorExpression, OperatorApplyGetCalledWithProperValues)
{
    auto op1 = basic_operator_fake{ };
    auto op2 = basic_operator_fake{ };
    auto op3 = basic_operator_fake{ };
    auto x   = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(op1, apply(0, 4, true)).Times(1);
    EXPECT_CALL(op2, apply(9, 4, false)).Times(1);
    EXPECT_CALL(op3, apply(9, 4, false)).Times(1);

    auto y = op3 * (op2 * op1) * x;
}


// The following unit tests are equivalent to the previous tests, to verify the
// interaction between the operator and the predicate in the same set of events.
//

TEST(OperatorExpression, PredicateDoApplyGetCalledOnce)
{
    using ::testing::_;
    using testing_operator = cvip::core::basic_operator<testing_predicate>;

    auto pr = testing_predicate{ };
    auto op = testing_operator{ pr };
    auto x  = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(pr, do_apply2(_, _, _)).Times(1);

    auto y = op * x;
}


TEST(OperatorExpression, PredicateDoApplyGetCalledAsFirstInChain)
{
    using ::testing::_;
    using testing_operator = cvip::core::basic_operator<testing_predicate>;

    auto pr = testing_predicate{ };
    auto op = testing_operator{ pr };
    auto x  = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(pr, do_apply2(_, _, true)).Times(1);

    auto y = op * x;
}


TEST(OperatorExpression, PredicateDoApplyGetCalledInProperOrder)
{
    using ::testing::_;
    using testing_operator = cvip::core::basic_operator<testing_predicate>;

    auto pr  = testing_predicate{ };
    auto op1 = testing_operator{ pr };
    auto op2 = testing_operator{ pr };
    auto x   = cvip::matrix(2, 2, CV_8UC1);

    EXPECT_CALL(pr, do_apply2(_, _, true)).Times(1);
    EXPECT_CALL(pr, do_apply2(_, _, false)).Times(2);

    auto y = op2 * op2 * op1 * x;
}


TEST(OperatorExpression, PredicateDoApplyGetCalledWithProperValues)
{
    using testing_operator = cvip::core::basic_operator<testing_predicate>;

    auto pr  = testing_predicate{ };
    auto op1 = testing_operator{ pr };
    auto op2 = testing_operator{ pr };
    auto op3 = testing_operator{ pr };
    auto x   = cvip::matrix(2, 2, CV_8UC1);

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
    EXPECT_CALL(pr, do_apply2(0, 4, true)).Times(1);
    EXPECT_CALL(pr, do_apply2(9, 4, false)).Times(2);

    auto y = op3 * (op2 * op1) * x;
}
