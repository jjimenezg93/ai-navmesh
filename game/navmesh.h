#ifndef _NAVMESH_H
#define _NAVMESH_H

#include <vector>

struct Polygon {
	struct Edge {
		int m_verticesID[2];
		Polygon * m_neighbour;
	};
	uint16_t m_id;
	std::vector<USVec2D> m_vertices;
	std::vector<Edge> m_edges;
};

class Navmesh {
public:
	Navmesh(const char * filename);

	uint16_t GetNumPolygons() const { return m_polygons.size(); }
	const Polygon * GetPolygon(uint16_t index) const { return &(m_polygons.at(index)); }
	Polygon * GetPolygon(uint16_t index) { return &(m_polygons.at(index)); }
private:
	std::vector<Polygon> m_polygons;
};

#endif //_NAVMESH_H