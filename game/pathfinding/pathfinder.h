#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

#include "../grid.h"

class PathNode {
public:
	PathNode(USVec2D pos, uint16_t cost, PathNode * parent): m_pos(pos), m_currentCost(cost),
		m_parentNode(parent) {}
	USVec2D GetPos() const { return m_pos; }
	uint16_t GetCost() const { return m_currentCost; }
	PathNode * GetParent() const { return m_parentNode; }
	void SetCost(uint16_t newCost) { m_currentCost = newCost; }
	void SetParent(PathNode * newParent) { m_parentNode = newParent; }
private:
	USVec2D m_pos;
	uint16_t m_currentCost; //costs are 10 vertical/horizontal and 14 diagonal
	PathNode * m_parentNode;
};

class Pathfinder: public virtual MOAIEntity2D {
public:
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();

	void SetStartPosition(float x, float y) { m_StartPosition = USVec2D(x, y); UpdatePath();}
	void SetEndPosition(float x, float y) { m_EndPosition = USVec2D(x, y); UpdatePath();}
	const USVec2D& GetStartPosition() const { return m_StartPosition;}
	const USVec2D& GetEndPosition() const { return m_EndPosition;}

    bool PathfindStep();
private:
	void UpdatePath();
private:
	void BuildPath(PathNode * lastNode);

	USVec2D m_StartPosition;
	USVec2D m_EndPosition;

	Grid m_grid;
	PathNode * m_startNode;
	PathNode * m_endNode;
	std::vector<PathNode> m_nodes;
	std::vector<PathNode *> m_openNodes;
	std::vector<PathNode *> m_closedNodes;

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);
};


#endif