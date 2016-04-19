#pragma once
#include "LiteralSyntax.h"

namespace flc
{
    namespace syntax
    {
        class DoubleLiteralSyntax : public LiteralSyntax<double>
        {
        public:
            DoubleLiteralSyntax(double value);
            ~DoubleLiteralSyntax();

            types::RuntimeType* getExpressionType() override;

            void emit(types::NameResolutionContextStack *ctx, emit::MethodBody *method) override;

            void stringify(stringstream* stream, int tabulation = 0) override;
        };
    }
}
