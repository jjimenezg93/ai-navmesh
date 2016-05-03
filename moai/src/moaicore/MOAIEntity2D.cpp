#include "pch.h"
#include "MOAIEntity2D.h"

#include <moaicore/MOAILayer.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIDeck.h>

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIDraw.h>

class MOAIEntity2DInternalData
{
public:
	MOAIEntity2DInternalData() : mDecks(NULL), maxDecks(0), numDecks(0) {}
	int addDeck(MOAIEntity2D* owner, MOAIDeck* deck);
private:
	void extendDecks(MOAIEntity2D* owner, int num_new_elements, int chunk = 12);

	friend class MOAIEntity2D;
	MOAILuaSharedPtr<MOAIProp> mProp;
	MOAILuaSharedPtr<MOAILayer> mLayer;
	MOAILuaSharedPtr<MOAIDeck>* mDecks;
	int numDecks;
	int maxDecks;
	MOAILuaSharedPtr<MOAIDeck> mDeck0;
	MOAILuaSharedPtr<MOAIDeck> mDeck1;
};

void MOAIEntity2DInternalData::extendDecks(MOAIEntity2D* owner, int num_new_elements, int chunk)
{
	if (numDecks + num_new_elements > maxDecks)
	{
		MOAILuaSharedPtr<MOAIDeck>* newDecks = new MOAILuaSharedPtr<MOAIDeck>[maxDecks + chunk];
		for (int i = 0; i < numDecks; ++i)
			newDecks[i].Set(*owner, mDecks[i].Get());
		if (mDecks)
			delete[] mDecks;
		mDecks = newDecks;
		maxDecks += chunk;
	}
}

int MOAIEntity2DInternalData::addDeck(MOAIEntity2D* owner, MOAIDeck* deck)
{
	extendDecks(owner, 1);
	mDecks[numDecks].Set(*owner, deck);
	return numDecks++;
}

MOAIEntity2D::MOAIEntity2D() : MOAIAction(), MOAIEntity()
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIAction)
        RTTI_EXTEND (MOAIEntity)
	RTTI_END

	mData = new MOAIEntity2DInternalData;
}

MOAIEntity2D::~MOAIEntity2D()
{
	mData->mLayer.Set(*this, 0);
	mData->mProp.Set(*this, 0);
	mData->mDeck0.Set(*this, 0);
	mData->mDeck1.Set(*this, 0);

	for (int i = 0; i < mData->numDecks; ++i)
	{
		mData->mDecks[i].Set(*this, 0);
	}
	delete[] mData->mDecks;
	delete mData;
}

bool MOAIEntity2D::IsDone()
{
	return false;
}

void MOAIEntity2D::OnStart()
{
}

void MOAIEntity2D::OnStop()
{

}

void MOAIEntity2D::OnUpdate (float step)
{
	//USVec3D position = GetLoc();
	//position.mY += step * 20;
	//SetLoc(position);
	//
	//float angle = GetRot();
	//angle += step * 20;
	//SetRot(angle);
}

//----------------------------------------------------------------//
void MOAIEntity2D::RegisterLuaClass(MOAILuaState& state)
{
	MOAIInstanceEventSource::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEntity2D::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIAction::RegisterLuaFuncs(state);

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{"setProp", _setProp},
		{"addImage", _addImage},
		{"setLoc", _setLoc},
		{"setRot", _setRot},
		{"setImage", _setImage},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int MOAIEntity2D::_setProp(lua_State* L)
{
	MOAI_LUA_SETUP( MOAIEntity2D, "U" )

	self->mData->mLayer.Set(*self, state.GetLuaObject<MOAILayer>(3, true));
	self->mData->mProp.Set(*self, state.GetLuaObject<MOAIProp>(2, true));

	self->mData->mLayer->InsertProp(self->mData->mProp.Get());

	return 0;
}


int MOAIEntity2D::_addImage(lua_State* L)
{
	MOAI_LUA_SETUP(MOAIEntity2D, "U")
	//if (!self->mData->mDeck0.Get())
	//{
	//	self->mData->mDeck0.Set(*self, state.GetLuaObject<MOAIDeck>(2, true));
	//}
	//else
	//{
	//	self->mData->mDeck1.Set(*self, state.GetLuaObject<MOAIDeck>(2, true));
	//}

	int deckIndex = self->mData->addDeck(self, state.GetLuaObject<MOAIDeck>(2, true));

	lua_pushinteger(L, deckIndex);
	
	//self->mData->mProp->SetDeck(self->mData->mDecks.Get());
	//self->mData->mDecks.push_back(value);

	return 1;
}


int MOAIEntity2D::_setLoc(lua_State* L)
{
	MOAI_LUA_SETUP( MOAIEntity2D, "U" )

	USVec3D loc;

	loc.mX = state.GetValue<float>(2, 0.0f);
	loc.mY = state.GetValue<float>(3, 0.0f);
	loc.mZ = state.GetValue<float>(4, 0.0f);

	self->SetLoc(loc);

	return 0;
}

int MOAIEntity2D::_setRot(lua_State* L)
{
	MOAI_LUA_SETUP( MOAIEntity2D, "U" )

	float angle = state.GetValue<float>(2, 0.0f);

	self->SetRot(angle);

	return 0;
}

int MOAIEntity2D::_setImage(lua_State* L)
{
	MOAI_LUA_SETUP(MOAIEntity2D, "U")

	int iIndex = state.GetValue<int>(2, 0);

	self->SetImage(iIndex);

	return 0;
}

void MOAIEntity2D::SetLoc(const USVec3D& position)
{
	mData->mProp->SetLoc(position);
	mData->mProp->ScheduleUpdate();
}

void MOAIEntity2D::SetRot(float angle)
{
	USVec3D rotation = mData->mProp->GetRot();
	rotation.mZ = angle;
	mData->mProp->SetRot(rotation);
	mData->mProp->ScheduleUpdate();
}

USVec3D MOAIEntity2D::GetLoc()
{
	return mData->mProp->GetLoc();
}

float MOAIEntity2D::GetRot()
{
	USVec3D rotation = mData->mProp->GetRot();
	return rotation.mZ;
}

void MOAIEntity2D::DrawDebug()
{
	//MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	//gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 1.0f);
	//MOAIDraw::DrawEllipseFill(0.0f, 0.0f, 5.0f, 5.0f, 20);
}

void MOAIEntity2D::SetImage(int index)
{
	if (mData->mDecks && mData->numDecks > index)
		mData->mProp->SetDeck(mData->mDecks[index]);

	//if (index == 0)
	//{
	//	mData->mProp->SetDeck(mData->mDeck0);
	//}
	//else
	//{
	//	mData->mProp->SetDeck(mData->mDeck1);
	//}
}

void MOAIEntity2D::RemoveEntity(MOAIEntity2D* entity)
{
	mData->mLayer.Get()->RemoveProp(entity->GetProp());
}

MOAIProp* MOAIEntity2D::GetProp()
{
	return mData->mProp.Get();
}