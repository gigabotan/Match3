#ifndef Grid_h__
#define Grid_h__

#include "cocos2d.h"
#include <vector>

#include "Gem.h"

class Grid : public cocos2d::Layer
{
protected:
	enum State {
		WAIT_ACTION,
		ACTION_PERFORMED,
		CHECKING,
		FALLING,
		GAME_END
	} m_state;

public:
	static Grid* create(size_t rows, int colors);

	bool init(size_t rows, int colors);

	virtual bool update();
	

protected:
	std::vector<cocos2d::Vector<Gem*>> m_gems;


};


#endif // Grid_h__
