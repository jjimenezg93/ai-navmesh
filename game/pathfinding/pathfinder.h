#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

#include "../navmesh.h"

class Polygon;

class PathNode {
public:
	PathNode(Polygon * pol, float cost, float estimCost, PathNode * parent = nullptr):
		m_polygon(pol),	m_totalCost(cost + estimCost), m_cost(cost + estimCost),
		m_estimatedCost(estimCost), m_parentNode(parent) {}
	USVec2D GetPos() const { return m_centre; }
	void SetCentrePos(USVec2D &pos) { m_centre = pos; }
	float GetTotalCost() const { return m_totalCost; }
	float GetCost() const { return m_cost; }
	float GetEstimated() const { return m_estimatedCost; }
	PathNode * GetParent() const { return m_parentNode; }
	Polygon * GetPolygon() const { return m_polygon; }
	void SetTotalCost(float newCost) { m_totalCost = newCost; }
	void SetCost(float newCost) { m_cost = newCost; }
	void SetEstimatedCost(float newCost) { m_estimatedCost = newCost; }
	void SetParent(PathNode * newParent) { m_parentNode = newParent; }
private:
	Polygon * m_polygon;
	USVec2D m_centre;
	//if any cost is -1 -> it's an obstacle
	float m_totalCost; //accumulated cost of path
	float m_cost; //own cost, independent of path
	float m_estimatedCost;
	PathNode * m_parentNode;
};

class Pathfinder: public virtual MOAIEntity2D {
public:
	static Pathfinder &Instance();
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();
	//those two methods receive grid coordinates
	void InitStartPosition(float x, float y);
	void InitEndPosition(float x, float y);
	//setters receive screen coordinates -> convert them to grid coords
	void SetStartPosition(float x, float y);
	void SetEndPosition(float x, float y);
	const USVec2D& GetStartPosition() const { return m_StartPosition;}
	const USVec2D& GetEndPosition() const { return m_EndPosition;}

	const std::vector<USVec2D> &GetPath() const;

    bool PathfindStep();
private:
	void UpdatePath();
	void BuildPath(PathNode * lastNode);

	static Pathfinder * m_pathfinder;

	PathNode * NearestNode(USVec2D &point);

	USVec2D m_StartPosition;
	USVec2D m_EndPosition;

	Navmesh m_navmesh;
	PathNode * m_startNode;
	PathNode * m_endNode;
	std::vector<PathNode> m_nodes;
	std::vector<PathNode *> m_openNodes;
	std::vector<PathNode *> m_closedNodes;
	std::vector<PathNode *> m_finalPath;
	std::vector<USVec2D> m_edgesPath; //edges points to calc pathfollowing

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _initStartPosition(lua_State* L);
	static int _initEndPosition(lua_State* L);
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);
};


#endif