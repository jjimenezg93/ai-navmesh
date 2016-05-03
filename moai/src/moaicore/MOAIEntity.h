#ifndef __ENTITYINTERNAL_H__
#define __ENTITYINTERNAL_H__

#include <moaicore/MOAILua.h>

class MOAIEntity: public virtual MOAILuaObject
{
public:
    DECL_LUA_FACTORY(MOAIEntity)
public:
    virtual void DrawDebug();

    MOAIEntity();
    ~MOAIEntity();
    void RegisterLuaClass (MOAILuaState& state);
    void RegisterLuaFuncs (MOAILuaState& state);
};
#endif