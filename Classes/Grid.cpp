#include "Grid.h"

USING_NS_CC;



Grid * Grid::create(size_t rows, int colors)
{
	return nullptr;
}

bool Grid::init(size_t rows, int colors)
{
	if (!Layer::init())
	{
		return false;
	}

	m_gems = std::vector<Vector<Gem*>>(rows, Vector<Gem*>(rows));

	return true;
}

bool Grid::update()
{
	return false;
}
