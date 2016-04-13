#include "stdafx.h"
#include "MethodGroup.h"

namespace flc
{
    namespace types
    {
        MethodGroup::MethodGroup(std::string name)
            : MethodGroup(name, nullptr, 0)
        {
        }
        MethodGroup::MethodGroup(std::string name, std::vector<MethodOverload*>* overloads)
            : MethodGroup(name, &(*overloads)[0], overloads->size())
        {
        }
        MethodGroup::MethodGroup(std::string name, MethodOverload** overloads, int overloadCount)
            : _name(name)
        {
            _overloads = new vector<MethodOverload*>();
            for (int q = 0; q < overloadCount; q++)
            {
                _overloads->push_back(overloads[q]);
            }
        }
        MethodGroup::~MethodGroup()
        {
            delete _overloads;
            _overloads = nullptr;
        }

        std::string MethodGroup::getName()
        {
            return _name;
        }
        NameType MethodGroup::getNameType()
        {
            return NameType::Member;
        }

        int MethodGroup::getOverloadCount()
        {
            return (int)_overloads->size();
        }
        MethodOverload* MethodGroup::getOverload(int idx)
        {
            //Let vector handle invalid indexes
            return _overloads->at(idx);
        }

        MethodOverload* MethodGroup::findOverload(RuntimeType** parameters, int parameterCount)
        {
            return findOverload(nullptr, parameters, parameterCount);
        }
        MethodOverload* MethodGroup::findOverload(RuntimeType* returnType, RuntimeType** parameters, int parameterCount)
        {
            for (int q = 0; q < getOverloadCount(); q++)
            {
                auto overload = getOverload(q);
                if (returnType != nullptr && !overload->getReturnType()->isSameAs(returnType)) continue;
                if (overload->isMatchForParameters(parameters, parameterCount)) return overload;
            }
            return nullptr;
        }
        MethodOverload* MethodGroup::findOverload(RuntimeType* returnType, ParameterInfo** parameters, int parameterCount)
        {
            for (int q = 0; q < getOverloadCount(); q++)
            {
                auto overload = getOverload(q);
                if (overload->getParameterCount() != parameterCount) return false;
                if (overload->getReturnType()->isSameAs(returnType) && overload->getParameterInfo(q)->isSameAs(parameters[q])) return overload;
            }
            return nullptr;
        }
    }
}