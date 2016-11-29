#ifndef Gem_h__
#define Gem_h__

#include "cocos2d.h"

class GameScene;

class Gem : public cocos2d::Node
{
public:
	static Gem* create(int type);

	bool init(int type);
	int getType();
	void changeType(int type);
	int getRow() const;
	void setRow(int row);
	int getCol() const;
	void setCol(int col);


private:
	cocos2d::DrawNode* m_drawNode = nullptr;
	int m_type;
	int m_row;
	int m_col;
};

struct GemComp
{
	bool operator()(const Gem* lhs, const Gem* rhs) const;
};

#endif // Gem_h__