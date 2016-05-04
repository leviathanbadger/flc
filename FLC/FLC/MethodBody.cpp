#include "stdafx.h"
#include "MethodBody.h"
#include "Instr.h"

namespace flc
{
    namespace emit
    {
        MethodBody::MethodBody()
        {
        }
        MethodBody::~MethodBody()
        {
        }

        void MethodBody::emit(Instr *instr)
        {
            instructions.push_back(instr);
        }
        void MethodBody::deleteInstructions()
        {
            for (size_t q = 0; q < instructions.size(); q++)
            {
                auto instr = instructions[q];
                delete instr;
            }
            instructions.clear();
        }
        const vector<Instr*> *MethodBody::getInstructions()
        {
            return &instructions;
        }

        string MethodBody::toString()
        {
            stringstream stream;
            for (auto instr : instructions)
            {
                instr->stringify(&stream);
                stream << endl;
            }
            return stream.str();
        }
    }
}
