#include <pch.h>

#include <moaicore/debug/MOAIDrawDebug.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIEntity2D.h>

#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

MOAIDrawDebug* MOAIDrawDebug::s_singleton = NULL;

MOAIDrawDebug::MOAIDrawDebug() : MOAIAction()
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIAction)
	RTTI_END
}

void MOAIDrawDebug::OnUpdate(float step)
{

}

void MOAIDrawDebug::DrawDebug()
{
	//MOAIDraw::Bind();
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );

	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );

	//gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 1.0f);
	//MOAIDraw::DrawLine(0.0f, 0.0f, 200.0f, 300.0f);

	for (EntityStore::iterator it = mEntities.begin(); it < mEntities.end(); ++it)
	{
		MOAIEntity2D* entity = (*it);
		entity->DrawDebug();
	}
}

MOAIDrawDebug* MOAIDrawDebug::Get()
{
	if (MOAIDrawDebug::s_singleton == NULL)
	{
		s_singleton = new MOAIDrawDebug();
	}
	return s_singleton;
}

void MOAIDrawDebug::Enable()
{
	MOAIDrawDebug* debugDraw = MOAIDrawDebug::Get();
	debugDraw->Start();
}

void MOAIDrawDebug::Disable()
{
	if (MOAIDrawDebug::s_singleton != NULL)
	{
		s_singleton->Stop();
		delete s_singleton;
		s_singleton = NULL;
	}
}

void MOAIDrawDebug::RegisterLuaClass(MOAILuaState& state)
{
	MOAIInstanceEventSource::RegisterLuaClass (state);

	luaL_Reg regTable [] = {
		{ "get",			_get },
		{ "enable",			_enable },
		{ "insertEntity",   _insertEntity},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

void MOAIDrawDebug::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIAction::RegisterLuaFuncs(state);
}

int MOAIDrawDebug::_get(lua_State* L)
{
	MOAIDrawDebug* instance = MOAIDrawDebug::Get();

	MOAILuaState state(L);
	state.Push(instance);
	return 1;
}

int MOAIDrawDebug::_enable(lua_State* L)
{
	MOAIDrawDebug* instance = MOAIDrawDebug::Get();
	instance->Enable();

	return 0;
}

int MOAIDrawDebug::_insertEntity(lua_State* L)
{
	MOAILuaState state(L);

	MOAIDrawDebug* instance = MOAIDrawDebug::Get();

	MOAIEntity2D* entity = state.GetLuaObject<MOAIEntity2D>(1, true);
	instance->mEntities.push_back(entity);

	return 0;
}