#include <stdafx.h>

#include "pathfinder.h"

const char * gridFilename = "grid_little.txt";
const uint16_t cellSize = 40; //size of each cell in pixels

Pathfinder::Pathfinder(): MOAIEntity2D(), m_grid{ gridFilename } {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	m_grid.IsObstacle(5, 5);
	uint16_t gridRows = m_grid.GetGridWidth();
	
	for (uint16_t i = 0; i < gridRows; ++i) {
		for (uint16_t j = 0; j < gridRows; ++j) {
			if (!m_grid.IsObstacle(i, j)) {
				USVec2D v(i, j);
				PathNode node(v, 0, nullptr);
				m_nodes.push_back(node);
			}
		}
	}
}

Pathfinder::~Pathfinder() {

}

void Pathfinder::UpdatePath() {
	m_openNodes.clear();
	m_closedNodes.clear();
}

void Pathfinder::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 1.0f);
	uint16_t gridRows = m_grid.GetGridWidth(); //also cols -> squared
	USRect r;
	r.mXMin = 0.f;
	r.mYMin = 0.f;
	r.mXMax = static_cast<float>(cellSize * gridRows);
	r.mYMax = static_cast<float>(cellSize * gridRows);

	MOAIDraw::DrawGrid(r, gridRows, gridRows);
	USRect fillCell;
	for (uint16_t i = 0; i < gridRows; ++i) {
		for (uint16_t j = 0; j < gridRows; ++j) {
			if (m_grid.IsObstacle(i, j)) {
				gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
				fillCell.mXMin = static_cast<float>(i * cellSize);
				fillCell.mYMin = static_cast<float>(j * cellSize);
				fillCell.mXMax = static_cast<float>(i * cellSize + cellSize);
				fillCell.mYMax = static_cast<float>(j * cellSize + cellSize);
				MOAIDraw::DrawRectFill(fillCell);
			}
		}
	}
	//startPos
	fillCell.mXMin = static_cast<float>(m_StartPosition.mX * cellSize);
	fillCell.mYMin = static_cast<float>(m_StartPosition.mY * cellSize);
	fillCell.mXMax = static_cast<float>(m_StartPosition.mX * cellSize + cellSize);
	fillCell.mYMax = static_cast<float>(m_StartPosition.mY * cellSize + cellSize);
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 1.0f);
	MOAIDraw::DrawRectFill(fillCell);

	//endPos
	fillCell.mXMin = static_cast<float>(m_EndPosition.mX * cellSize);
	fillCell.mYMin = static_cast<float>(m_EndPosition.mY * cellSize);
	fillCell.mXMax = static_cast<float>(m_EndPosition.mX * cellSize + cellSize);
	fillCell.mYMax = static_cast<float>(m_EndPosition.mY * cellSize + cellSize);
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 1.0f);
	MOAIDraw::DrawRectFill(fillCell);
}

bool Pathfinder::PathfindStep() {
    // returns true if pathfinding process finished
    return true;
}

//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state) {
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
        { "pathfindStep",           _pathfindStep},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(3, 0.0f);
	float pY = state.GetValue<float>(2, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(3, 0.0f);
	float pY = state.GetValue<float>(2, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L) {
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}