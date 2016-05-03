#ifndef __MOAI_DRAW_DEBUG_H__
#define __MOAI_DRAW_DEBUG_H__

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIProp.h>

class MOAIEntity2D;

class MOAIDrawDebug: public virtual MOAIAction
{
public:
	DECL_LUA_FACTORY (MOAIDrawDebug)

	MOAIDrawDebug();

	static MOAIDrawDebug* Get();
	
	static void Enable();
	static void Disable();

	virtual void OnUpdate(float step);
	virtual bool IsDone() { return false;}
	
	virtual void DrawDebug();

	virtual void RegisterLuaClass(MOAILuaState& state);
	virtual void RegisterLuaFuncs(MOAILuaState& state);

private:
	static MOAIDrawDebug* s_singleton;

	static int _get(lua_State* L);
	static int _enable(lua_State* L);
	static int _insertEntity(lua_State* L);

	typedef std::vector<MOAIEntity2D* > EntityStore;
	EntityStore mEntities;
};

#endif