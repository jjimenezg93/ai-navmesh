#include <stdafx.h>
#include <tinyxml.h>

#include "navmesh.h"

Navmesh::Navmesh(const char * filename) {
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) {
		fprintf(stderr, "Couldn't read params from %s", filename);
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem) {
		fprintf(stderr, "Invalid format for %s", filename);
	}

	TiXmlHandle hMesh(pElem);

	TiXmlHandle hPolygons = hMesh.FirstChildElement("polygons");
	TiXmlHandle hLinks = hMesh.FirstChildElement("links");
	Polygon polygon;
	USVec2D vert;

	int16_t polygonIndex = 0;
	int16_t pointIndex = 0;
	while (polygonIndex != -1) {
		TiXmlElement * polygonElem = hPolygons.Child("polygon", polygonIndex).Element();
		if (polygonElem) {
			while (pointIndex != -1) {
				TiXmlElement * pointElem =
					hPolygons.Child("polygon", polygonIndex).Child(pointIndex).Element();
				if (pointElem) {
					polygon.m_id = polygonIndex;
					pointElem->Attribute("x", &vert.mX);
					pointElem->Attribute("y", &vert.mY);
					polygon.m_vertices.push_back(vert);
					++pointIndex;
				} else {
					pointIndex = -1;
				}
			}
			pointIndex = 0;
			m_polygons.push_back(polygon);
			polygon.m_vertices.clear(); //if not, previous vertices accumulate for next polygons
			++polygonIndex;
		} else {
			polygonIndex = -1;
		}
	}
	int16_t linkIndex = 0;
	Polygon::Edge e;
	int p1;
	int p2;
	while (linkIndex != -1) {
		TiXmlElement * linkElem = hLinks.Child("link", linkIndex).Element();
		if (linkElem) {
			TiXmlElement * linkStartElem =
				hLinks.Child("link", linkIndex).FirstChildElement("start").Element();
			TiXmlElement * linkEndElem =
				hLinks.Child("link", linkIndex).FirstChildElement("end").Element();
			for (std::vector<Polygon>::iterator itr = m_polygons.begin(); itr != m_polygons.end();
			++itr) {
				linkStartElem->Attribute("polygon", &p1);
				linkEndElem->Attribute("polygon", &p2);
				if (p1 == itr->m_id) {
					linkStartElem->Attribute("edgestart", &e.m_verticesID[0]);
					linkStartElem->Attribute("edgeend", &e.m_verticesID[1]);
					e.m_neighbour = &(m_polygons.at(p2));
					itr->m_edges.push_back(e);
				} else if (p2 == itr->m_id) {
					linkEndElem->Attribute("edgestart", &e.m_verticesID[0]);
					linkEndElem->Attribute("edgeend", &e.m_verticesID[1]);
					e.m_neighbour = &(m_polygons.at(p1));
					itr->m_edges.push_back(e);
				}
			}
			++linkIndex;
		} else {
			linkIndex = -1;
		}
	}
}

