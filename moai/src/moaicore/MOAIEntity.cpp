#include "pch.h"
#include "MOAIEntity.h"

MOAIEntity::MOAIEntity() : MOAILuaObject()
{
    RTTI_BEGIN
        RTTI_EXTEND(MOAILuaObject)
    RTTI_END
}

MOAIEntity::~MOAIEntity()
{
}

//----------------------------------------------------------------//
void MOAIEntity::RegisterLuaClass(MOAILuaState& state)
{
    MOAILuaObject::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEntity::RegisterLuaFuncs(MOAILuaState& state)
{
    MOAILuaObject::RegisterLuaFuncs(state);

    luaL_Reg regTable [] = {
        { NULL, NULL }
    };

    luaL_register(state, 0, regTable);
}

void MOAIEntity::DrawDebug()
{
}