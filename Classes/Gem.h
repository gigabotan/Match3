#ifndef Gem_h__
#define Gem_h__

#include "cocos2d.h"

class GameScene;

class Gem : public cocos2d::Node
{
public:
	static Gem* create(int type, int width);

	bool init(int type, int width);
	int getType();
	void changeType(int type);
	int getRow() const;
	void setRow(int row);
	int getCol() const;
	void setCol(int col);


private:
	static const std::vector<cocos2d::Color4F> m_colors;
    
	cocos2d::DrawNode* m_drawNode = nullptr;
	int m_width;
	int m_type;
	int m_row;
	int m_col;
};


// comparator for matched elements set with right order
struct GemComp
{
	bool operator()(const Gem* lhs, const Gem* rhs) const;
};



#endif // Gem_h__
