#ifndef _GRID_H_
#define _GRID_H_

#include <vector>

class Grid {
public:
	Grid(const char * filename);

	bool IsObstacle(uint16_t x, uint16_t y);
	uint16_t GetNumOfCells() const { return static_cast<uint16_t>(m_cells.size()); }
	uint16_t GetGridWidth() const { return m_gridWidth; } //guessing square grids -> also Height
private:
	struct Cell {
		USVec2D m_position;
		bool m_obstacle;
	};
	std::vector<Cell> m_cells;
	uint16_t m_gridWidth;
};

#endif //!_GRID_H_