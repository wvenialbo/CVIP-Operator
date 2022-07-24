# Summary

## Image operators rationale

Overloads of the built-int multiplication operator, `:::cpp operator*()`, are
used to specify operations among image operators and images.

Multiplication is a binary operation represented by the infix operator
`_ * _`{.none} and the native behaviour of `:::cpp operator*()`, when
operations are written one after another without any associativity
indication, is to operate associating its operands from left to right,
that is:

\[
    a * b * c * d \equiv ((a * b) * c) * d.
\]

Conversely, we want operations among image operators and image matrices to be
rigth associative, so the notation can be consistent with the matrix
representation of linear operators and state vectors, in which the operand is
written at the right end of the operator expression.[^1]

[^1]: Since linear operators are plain matrices, and its operands are also
      matrices, there makes no difference the order in which the operations
      among the operators and the operand are done, provided they preserve
      their general order; remember that matrix multiplication are not always
      commutative.

A linear operator acting on a state is a matrix times a vector. The product of
operators is the product of matrices. Operators which donʼt commute are
represented by matrices that don't commute. On the other hand, image operators
are not linear nor they commute; or they rarely do. Therefore, to achieve our
goal, we need to introduce a new entity, an operator expression, thus, by our
part, an image operator acting on an image is an operator expression times an
image and the product of operators is an operator expression. Conceptually, an
image operator is an operator expression by itself.

### Product of an operator and an image

Applying an operator, \(P_1\)&nbsp;, on a image (matrix), \(M_1\)&nbsp;,
produces a new image, \(M_2\)&nbsp;:

\[
    P_1 * M_1 \longrightarrow M_2.
\]

### Product of operators

Multiplying two operators, \(P_2\)&nbsp; and \(P_1\)&nbsp;, produces an operator
expression, \(\text{OpExpr}(P_1, P_2)\)&nbsp;&nbsp;&nbsp;, that gather both
operators for later application in the appropriate order. The operator on the
ritght is pushed to the front of the operator list while the operator on the
left is pushed to the back:

\[
    P_2 * P_1 \longrightarrow \text{OpExpr}(P_1, P_2).
\]

### Product of an operator expression and an image

Applying an operator expression,
\(\text{OpExpr}(P_1,P_2,\dots,P_n)\)&nbsp;&nbsp;&nbsp;&nbsp;, on a image,
\(M_1\)&nbsp;, produces a new image, \(M_2\)&nbsp;, after applying the
encapsulated operators in the order they appear in the list of operators:

\[
    \text{OpExpr}(P_1, P_2, \dots, P_n) * M_1 \longrightarrow P_n * (\cdots * (P_2 * (P_1 * M_1)))  \longrightarrow M_2.
\]

### Product of operators and operator expressions

Applying an operator expression, \(\text{OpExpr}(P_1, P_2)\)&nbsp;&nbsp;&nbsp;,
on an operator, \(P_3\)&nbsp;, produces a new operator expression pushing the
operator to the front of the operator list:

\[
    \text{OpExpr}(P_1, P_2) * P_3 \longrightarrow \text{OpExpr}(P_3, P_1, P_2).
\]

Premultiplying an operator, \(P_3\)&nbsp;, with an operator expression,
\(\text{OpExpr}(P_1, P_2)\)&nbsp;&nbsp;&nbsp;, produces a new operator
expression pushing the operator to the back of the operator list:

\[
    P_3 * \text{OpExpr}(P_1, P_2) \longrightarrow \text{OpExpr}(P_1, P_2, P_3).
\]

### Product of operator expressions

Multiplying two operator expressions, \(\text{OpExpr}(P_1,
P_2)\)&nbsp;&nbsp;&nbsp; and \(\text{OpExpr}(P_3, P_4)\)&nbsp;&nbsp;&nbsp;,
produces a new operator expression concatenating their operator list in the
proper order, the operators from the left operand are enqueued to those of the
right operand:

\[
    \text{OpExpr}(P_1, P_2) * \text{OpExpr}(P_3, P_4) \longrightarrow \text{OpExpr}(P_3, P_4, P_1, P_2).
\]

### Properties of operator expressions

#### Commutativity

The commutative property does not hold in general for operators, i.e.

\[
    P_2 * P_1 \neq P_1 * P_2,
\]

in fact,

\[
    P_2 * P_1 \longrightarrow \text{OpExpr}(P_1, P_2) \neq P_1 * P_2 \longrightarrow \text{OpExpr}(P_2, P_1).
\]

Examples showing the same for operator expressions has already given above.

!!! note

    One inmediate exception is the intensity inverter (or negative filter)
    operator. Since it is its own inverse, it can commute with itself. The
    same is not true for other operators.

#### Associativity

Associative property among operators and operator expressions is just syntactic
sugar and is implemented for semantic purposes, e.g. for grouping or emphasise
related operations. The following operations produce the same result:

\[
    (P_1 * P_2) * P_3 * M = P_1 * (P_2 * P_3) * M = P_1 * P_2 * (P_3 * M) = P_1 * P_2 * P_3 * M.
\]

The compiler produces the same code for the first and last members, however, for
the second and third members, the intermediate result produced by the compiler
is not the same.

Applying association to operators or operator expressions and matrices, force
the application of the associated operations and produces new intermediate
results before continuing with the remaning operators.
