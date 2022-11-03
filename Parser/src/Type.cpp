#include "Type.h"
#include <sstream>

IntType TypeSystem::commonInt = IntType(4);
FloatType TypeSystem::commonFloat = FloatType(4); // 应该是字节大小吧
VoidType TypeSystem::commonVoid = VoidType();

Type* TypeSystem::intType = &commonInt;
Type* TypeSystem::floatType = &commonFloat;
Type* TypeSystem::voidType = &commonVoid;

std::string IntType::toStr()
{
    return "int";
}

std::string FloatType::toStr()
{
    return "float";
}

std::string VoidType::toStr()
{
    return "void";
}

std::string FunctionType::toStr()
{
    std::ostringstream buffer;
    buffer << returnType->toStr() << "()";
    return buffer.str();
}
