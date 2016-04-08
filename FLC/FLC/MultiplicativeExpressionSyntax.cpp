#include "stdafx.h"
#include "MultiplicativeExpressionSyntax.h"

namespace flc
{
    namespace syntax
    {
        MultiplicativeExpressionSyntax::MultiplicativeExpressionSyntax(ExpressionSyntax* left, string op, ExpressionSyntax* right)
            : _left(left), _right(right)
        {
            if (op == "*") _op = MultiplicativeOperator::Multiply;
            else if (op == "/") _op = MultiplicativeOperator::Divide;
            else if (op == "%") _op = MultiplicativeOperator::Remainder;
            else
            {
                reportError("Invalid Multiplicative Operator in MultiplicativeExpressionSyntax::ctor: " + op);
                _op = MultiplicativeOperator::ErrorState;
            }
        }
        MultiplicativeExpressionSyntax::~MultiplicativeExpressionSyntax()
        {
        }

        ExpressionSyntax* MultiplicativeExpressionSyntax::getLeftOperand()
        {
            return _left;
        }
        ExpressionSyntax* MultiplicativeExpressionSyntax::getRightOperand()
        {
            return _right;
        }
        MultiplicativeOperator MultiplicativeExpressionSyntax::getOperator()
        {
            return _op;
        }



        void MultiplicativeExpressionSyntax::stringify(stringstream* stream, int tabulation)
        {
            _left->stringify(stream, tabulation);
            switch (_op)
            {
            case MultiplicativeOperator::Multiply:
                *stream << " * ";
                break;
            case MultiplicativeOperator::Divide:
                *stream << " / ";
                break;
            case MultiplicativeOperator::Remainder:
                *stream << " % ";
                break;
            case MultiplicativeOperator::ErrorState:
            default:
                *stream << " %%ERROR%% ";
                break;
            }
            _right->stringify(stream, 0);
        }
    }
}
