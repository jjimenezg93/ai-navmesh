#ifndef __ENTITY2DINTERNAL_H__
#define __ENTITY2DINTERNAL_H__

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIEntity.h>
#include <moaicore/MOAIProp.h>

class MOAIEntity2DInternalData;
class MOAIEntity2D : public virtual MOAIAction, public virtual MOAIEntity
{
public:
	DECL_LUA_FACTORY(MOAIEntity2D)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual bool IsDone();
	virtual void DrawDebug();

	MOAIEntity2D();
	~MOAIEntity2D();
	void RegisterLuaClass (MOAILuaState& state);
	void RegisterLuaFuncs (MOAILuaState& state);

	void SetLoc(const USVec3D& position);
	void SetRot(float angle);

	USVec3D GetLoc();
	float GetRot();

	void SetImage(int index);
	void RemoveEntity(MOAIEntity2D* entity);
	void SetTint(USColorVec tint);

	MOAIProp* GetProp();
private:
	static int _setProp(lua_State* L);
	static int _addImage(lua_State* L);
	static int _setLoc(lua_State* L);
	static int _setRot(lua_State* L);
	static int _setImage(lua_State* L);
	static int _setTint(lua_State* L);

	
private:
	MOAIEntity2DInternalData* mData;
};
#endif