#include "stdafx.h"
#include "grid.h"

//in bytes
const uint16_t maxFileSize = 2000;

Grid::Grid(const char * filename) {
	m_gridWidth = 0;

	FILE * file = fopen(filename, "rb");

	fseek(file, 0, SEEK_END);
	const size_t numElementsToRead = ftell(file);
	fseek(file, 0, SEEK_SET);

	char buffer[maxFileSize];
	fread(&buffer, sizeof(char), numElementsToRead, file);
	buffer[numElementsToRead] = '\0';

	uint16_t x = 0, y = 0;
	Cell c;
	char ch;
	for (uint16_t i = 0; i < numElementsToRead; ++i) {
		ch = buffer[i];
		if (ch == '.') {
			c.m_obstacle = false;
		} else if (ch == '#') {
			c.m_obstacle = true;
		} else {
			if (m_gridWidth == 0) {
				m_gridWidth = x;
			}
			x = 0;
			//i++; //manually pass \n
			y++;
			continue;
		}
		c.m_position.mX = x++;
		c.m_position.mY = y;
		m_cells.push_back(c);
	}

	c = c;
}

bool Grid::IsObstacle(uint16_t x, uint16_t y) {
	return m_cells.at(x + y * m_gridWidth).m_obstacle;
}

/*
........
........
....#...
........
..#.#...
....#...
....#...
........
*/