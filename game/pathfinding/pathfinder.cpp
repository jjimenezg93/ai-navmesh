#include <stdafx.h>

#include "pathfinder.h"
#include "navmesh.h"

const char * navmeshFilename = "navmesh.xml";

Pathfinder * Pathfinder::m_pathfinder = nullptr;

Pathfinder &Pathfinder::Instance() {
	if (!m_pathfinder)
		m_pathfinder = new Pathfinder();
	return *m_pathfinder;
}

Pathfinder::Pathfinder(): MOAIEntity2D(), m_navmesh{navmeshFilename} {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
		RTTI_END

	m_pathfinder = this;
	//fill m_nodes
	uint16_t numPolygons = m_navmesh.GetNumPolygons();
	float cost = 0;
	USVec2D diagonal;
	float centreDist;
	for (uint16_t index = 0; index < numPolygons; ++index) {
		cost = rand() % 3000 + 500;
		PathNode node(m_navmesh.GetPolygon(index), cost, 0.f);
		diagonal = node.GetPolygon()->m_vertices[2] - node.GetPolygon()->m_vertices[0];
		centreDist = diagonal.Length() / 2;
		node.SetCentrePos(node.GetPolygon()->m_vertices[0] + (diagonal.NormVector() * centreDist));
		m_nodes.push_back(node);
	}
}

Pathfinder::~Pathfinder() {

}

const std::vector<USVec2D> &Pathfinder::GetPath() const {
	return m_edgesPath;
}

PathNode * Pathfinder::NearestNode(USVec2D &point) {
	float currDist;
	float minDist = (m_nodes.begin()->GetPos() - point).Length();
	PathNode * nearest = &(*m_nodes.begin());
	for (std::vector<PathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		currDist = (itr->GetPos() - point).Length();
		if (currDist <= minDist) {
			minDist = currDist;
			nearest = &(*itr);
		}
	}
	return nearest;
}

void Pathfinder::UpdatePath() {
	//need to set all parents to null before recalculating the path.
	//if not, there's an infinite loop in BuildPath()
	for (std::vector<PathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		itr->SetParent(nullptr);
	}

	//set StartNode and EndNode
	m_startNode = NearestNode(m_StartPosition);
	m_endNode = NearestNode(m_EndPosition);

	//recalculate all estimated costs
	int16_t cost = 0;
	PathNode * node;
	for (std::vector<PathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		node = &(*itr);
		if (node->GetCost() != -1) {
			//estimated based on pixels distance
			float cost = node->GetCost();
			float estimated = (sqrt(pow((m_endNode->GetPos().mX - node->GetPos().mX), 2)
				+ pow(m_endNode->GetPos().mY - node->GetPos().mY, 2)));
			node->SetEstimatedCost(estimated);
			node->SetCost(cost + estimated);
			node->SetTotalCost(cost + estimated);
		}
	}

	//A*
	m_openNodes.clear();
	m_closedNodes.clear();
	m_startNode->SetCost(0);
	m_startNode->SetTotalCost(0);
	m_openNodes.push_back(m_startNode);
	bool objectiveFound = false;
	while (!m_openNodes.empty() && !objectiveFound) {
		PathNode * node = *(m_openNodes.begin());
		m_openNodes.erase(m_openNodes.begin());
		if (node->GetPos().mX == m_endNode->GetPos().mX
			&& node->GetPos().mY == m_endNode->GetPos().mY) {
			BuildPath(node);
		} else {
			for (std::vector<Polygon::Edge>::iterator itr = node->GetPolygon()->m_edges.begin();
			itr != node->GetPolygon()->m_edges.end(); ++itr) {
				printf("EDGE: %d - %d\n", node->GetPolygon()->m_id, itr->m_neighbour->m_id);
				PathNode * nextNode = nullptr;
				for (std::vector<PathNode>::iterator pathNodeItr = m_nodes.begin();
				pathNodeItr != m_nodes.end(); ++pathNodeItr) {
					if (pathNodeItr->GetPolygon()->m_id == itr->m_neighbour->m_id) {
						nextNode = &(*pathNodeItr);
					}
				}
				if (nextNode == m_endNode) {
					nextNode->SetParent(node);
					nextNode->SetTotalCost(node->GetTotalCost() + nextNode->GetCost());
					BuildPath(nextNode);
					objectiveFound = true;
					break;
				}
				if (nextNode->GetCost() != -1) {
					if (node == nextNode) {
						continue;
					} else if (find(m_closedNodes.begin(), m_closedNodes.end(), nextNode)
						!= m_closedNodes.end()) {
						continue;
					} else if (find(m_openNodes.begin(), m_openNodes.end(), nextNode)
						!= m_openNodes.end()) {
						if (nextNode->GetTotalCost() > 0
							&& nextNode->GetTotalCost() >
							node->GetTotalCost() + nextNode->GetCost()) {
							nextNode->SetTotalCost(node->GetTotalCost() + nextNode->GetCost());
							nextNode->SetParent(node);
						}
					} else {
						nextNode->SetParent(node);
						m_openNodes.push_back(nextNode);
					}
				}
			}
			std::vector<PathNode *>::iterator el = find(m_openNodes.begin(),
				m_openNodes.end(), node);
			if (el != m_openNodes.end()) {
				m_openNodes.erase(el);
			}
			m_closedNodes.push_back(node);
		}
	}
}

void Pathfinder::BuildPath(PathNode * lastNode) {
	m_finalPath.clear();
	m_edgesPath.clear();
	//iterate over all node parents to get the full path
	PathNode * node = lastNode;
	USVec2D edgeDir;
	USVec2D edgePos;
	float edgeLength;
	while (node->GetParent()) {
		m_finalPath.push_back(node);
		for (std::vector<Polygon::Edge>::iterator itr = node->GetPolygon()->m_edges.begin();
		itr != node->GetPolygon()->m_edges.end(); ++itr) {
			if (itr->m_neighbour == node->GetParent()->GetPolygon()) {
				edgeDir = node->GetPolygon()->m_vertices[itr->m_verticesID[1]]
					- node->GetPolygon()->m_vertices[itr->m_verticesID[0]];
				edgeLength = edgeDir.Length();
				edgePos = node->GetPolygon()->m_vertices[itr->m_verticesID[0]]
					+ (edgeDir.NormVector() * (edgeLength / 2));
				m_edgesPath.push_back(edgePos);
			}
		}
		node = node->GetParent();
	}
	node = node;
}

void Pathfinder::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	uint16_t numPolygons = m_navmesh.GetNumPolygons();
	Polygon * polygon;
	for (uint16_t i = 0; i < numPolygons; ++i) {
		polygon = m_navmesh.GetPolygon(i);
		gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.1f);
		MOAIDraw::DrawPolygonFilled(polygon->m_vertices);
		gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.1f);
		MOAIDraw::DrawPolygon(polygon->m_vertices);
	}

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.1f);
	MOAIDraw::DrawEllipseFill(m_StartPosition.mX, m_StartPosition.mY, 5, 5, 32);
	gfxDevice.SetPenColor(1.0f, 0.0f, 1.0f, 0.1f);
	MOAIDraw::DrawEllipseFill(m_EndPosition.mX, m_EndPosition.mY, 5, 5, 32);

	//draw Polygons path
	for (std::vector<PathNode *>::iterator pathItr = m_finalPath.begin();
	pathItr != m_finalPath.end(); ++pathItr) {
		gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.1f);
		MOAIDraw::DrawPolygonFilled((*pathItr)->GetPolygon()->m_vertices);
	}

	//draw Edges path
	std::vector<USVec2D>::iterator edgeItr = m_edgesPath.begin();
	uint16_t edgeIt = 0;
	while (edgeIt < m_edgesPath.size() - 1) {
		gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.1f);
		MOAIDraw::DrawLine(m_edgesPath.at(edgeIt), m_edgesPath.at(edgeIt++));
	}

	//startNode
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.1f);
	MOAIDraw::DrawPolygonFilled(m_startNode->GetPolygon()->m_vertices);

	//endNode
	gfxDevice.SetPenColor(1.0f, 0.0f, 1.0f, 0.1f);
	MOAIDraw::DrawPolygonFilled(m_endNode->GetPolygon()->m_vertices);
}

void Pathfinder::InitStartPosition(float x, float y) {
	m_StartPosition = USVec2D(x, y);
	UpdatePath();
}

void Pathfinder::InitEndPosition(float x, float y) {
	m_EndPosition = USVec2D(x, y);
	UpdatePath();
}

void Pathfinder::SetStartPosition(float x, float y) {
	m_StartPosition = USVec2D(x, y);
	UpdatePath();
}
void Pathfinder::SetEndPosition(float x, float y) {
	m_EndPosition = USVec2D(x, y);
	UpdatePath();
}

bool Pathfinder::PathfindStep() {
	// returns true if pathfinding process finished
	return true;
}

//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state) {
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable[] = {
		{"initStartPosition", _initStartPosition},
		{"initEndPosition", _initEndPosition},
		{"setStartPosition", _setStartPosition},
		{"setEndPosition", _setEndPosition},
		{"pathfindStep", _pathfindStep},
		{NULL, NULL}
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_initStartPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->InitStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_initEndPosition(lua_State* L) {
	MOAI_LUA_SETUP(Pathfinder, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->InitEndPosition(pX, pY);
	return 0;
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