#include "stdafx.h"
#include "AndExpressionSyntax.h"
#include "BinaryOperator.h"

namespace flc
{
    namespace syntax
    {
        AndExpressionSyntax::AndExpressionSyntax(ExpressionSyntax* left, ExpressionSyntax* right)
            : _left(left), _right(right)
        {
        }
        AndExpressionSyntax::~AndExpressionSyntax()
        {
        }

        ExpressionSyntax* AndExpressionSyntax::getLeftOperand()
        {
            return _left;
        }
        ExpressionSyntax* AndExpressionSyntax::getRightOperand()
        {
            return _right;
        }

        void AndExpressionSyntax::resolveTypes(types::NameResolutionContextStack *ctx)
        {
            _left->resolveTypes(ctx);
            _right->resolveTypes(ctx);

            op::BinaryOperator *bin_op = op::Operator::bitwiseAnd();
            _overload = bin_op->findOverload(_left->getExpressionType(), _right->getExpressionType());

            if (_overload != nullptr)
            {
                _left->suggestExpressionType(_overload->getParameterInfo(0)->getType());
                _right->suggestExpressionType(_overload->getParameterInfo(1)->getType());
                _left->resolveTypes(ctx);
                _right->resolveTypes(ctx);
            }
        }
        types::RuntimeType* AndExpressionSyntax::getExpressionType()
        {
            if (_overload == nullptr) return nullptr;
            return _overload->getReturnType();
        }



        void AndExpressionSyntax::stringify(stringstream* stream, int tabulation)
        {
            _left->stringify(stream, tabulation);
            *stream << " & ";
            _right->stringify(stream, 0);
        }
    }
}
