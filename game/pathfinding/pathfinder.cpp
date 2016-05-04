#include <stdafx.h>

#include "pathfinder.h"

const char * gridFilename = "grid_little.txt";
const uint16_t cellSize = 40; //size of each cell in pixels

Pathfinder::Pathfinder(): MOAIEntity2D(), m_grid{gridFilename} {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	m_grid.IsObstacle(5, 5);
	uint16_t gridRows = m_grid.GetGridWidth();

	for (uint16_t i = 0; i < gridRows - 1; ++i) {
		for (uint16_t j = 0; j < gridRows - 1; ++j) {
			if (!m_grid.IsObstacle(i, j)) {
				USVec2D v(i, j);
				PathNode node(v, 0, nullptr);
				m_nodes.push_back(node);
			}
		}
	}

	/*v.mX = m_EndPosition.mX;
	v.mY = m_EndPosition.mY;
	m_endNode = new PathNode(v, 0, nullptr);*/
}

Pathfinder::~Pathfinder() {

}

void Pathfinder::UpdatePath() {
	if (m_StartPosition.mX >= 0 && m_StartPosition.mY >= 0 && m_EndPosition.mX >= 0
	&& m_EndPosition.mY >= 0) { //need to check both Start and End positions are at least 0
		uint8_t pos = m_StartPosition.mX * (m_grid.GetGridWidth() - 1) + m_StartPosition.mY;
		m_startNode = &m_nodes.at(pos);

		pos = m_EndPosition.mX * (m_grid.GetGridWidth() - 1) + m_EndPosition.mY;
		m_endNode = &m_nodes.at(pos);

		m_openNodes.clear();
		m_closedNodes.clear();

		/*m_openNodes.push_back(m_startNode);
		while (!m_openNodes.empty()) {
			PathNode * node = m_openNodes.back();
			if (node->GetPos().mX == m_endNode->GetPos().mX
				&& node->GetPos().mY == m_endNode->GetPos().mY) {
				BuildPath(node);
			} else {
				//with 2 for loops from -1 to 1, we iterate over all adjacent nodes
				for (int8_t x = -1; x <= 1; ++x) {
					for (int8_t y = -1; y <= 1; ++y) {
						m_nodes.
					}
				}
			}
		}*/
	}
}

void Pathfinder::BuildPath(PathNode * lastNode) {
	//iterate over all node parents to get the full path
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

	luaL_Reg regTable[] = {
		{"setStartPosition", _setStartPosition},
		{"setEndPosition", _setEndPosition},
		{"pathfindStep", _pathfindStep},
		{NULL, NULL}
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

		self->PathfindStep();
	return 0;
}