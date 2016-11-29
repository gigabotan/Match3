#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Gem.h"

class GameScene : public cocos2d::Layer
{
protected:
	enum State {
		WAIT_ACTION,
		ACTION_PERFORMED,
		SWAPPING,
		SWAPPED,
		FALLING,
		GAME_END
	} m_state;
public:

    static cocos2d::Scene* createSceneWithRowsColors(int rows, int cols, int colors);
	static GameScene* create(int rows, int cols, int colors);

    bool init(int rows, int cols,  int colors);

	void clearGrid();
	void fillGrid();
	void addRandomGem(int col);
	

	void removeMatches();
	void updateColumns();
	bool lookForMatches();
	bool lookForPossibleMoves();
	std::set<Gem*, GemComp> getMatchHoriz(int col, int row);
	std::set<Gem*, GemComp> getMatchVert(int col, int row);
	bool matchPattern(int col, int row, std::vector<int> neighbour, std::vector<std::vector<int>> possible);
	bool matchType(int col, int row, int type);


	void makeSwap(Gem* first, Gem* second);
	void swapGems(Gem* first, Gem* second);
	void onSwapEnd(Gem* first, Gem* second);
	void onGemTouch(Gem* gem);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);



protected:

	static const cocos2d::Vec2 m_offset;
	static const int m_stride = 15;

	int m_rows;
	int m_cols;
	int m_colors;
	std::vector<cocos2d::Vector<Gem*>> m_grid;
	std::set<Gem*, GemComp> m_matched;

	Gem* m_first = nullptr;
	bool m_userMoved;
    
};

#endif // __GAME_SCENE_H__
