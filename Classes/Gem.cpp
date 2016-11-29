#include "Gem.h"

#include "GameScene.h"

USING_NS_CC;

static const std::vector<cocos2d::Color4F> colors = { cocos2d::Color4F::BLACK, \
														cocos2d::Color4F::RED, \
														cocos2d::Color4F::GREEN, \
														cocos2d::Color4F::BLUE, \
														cocos2d::Color4F::YELLOW, \
														cocos2d::Color4F::MAGENTA, \
														cocos2d::Color4F::ORANGE, \
														cocos2d::Color4F::GRAY };



Gem * Gem::create(int type)
{
	Gem * ret = new (std::nothrow) Gem();
	if (ret && ret->init(type))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool Gem::init(int type)
{
	if (!Node::init())
	{
		return false;
	}

	m_type = type;
	
	CCLOG("anchor:%.2f, %.2f", getAnchorPoint().x, getAnchorPoint().y);
	this->setContentSize(Size(8, 8));
	this->setAnchorPoint(Vec2(0.5, 0.5));
	m_drawNode = DrawNode::create();
	m_drawNode->drawSolidRect(Vec2(0, 0), Vec2(8, 8), colors[m_type]);
	addChild(m_drawNode);
	

	return true;
}

int Gem::getType()
{
	return m_type;
}

void Gem::changeType(int type)
{
	m_drawNode->clear();
	m_type = type;
	m_drawNode->drawSolidRect(Vec2(-4, -4), Vec2(4, 4), colors[m_type]);
}

int Gem::getRow() const
{
	return m_row;
}

void Gem::setRow(int row)
{
	m_row = row;
}

int Gem::getCol() const
{
	return m_col;
}

void Gem::setCol(int col)
{
	m_col = col;
}



bool GemComp::operator()(const Gem * lhs, const Gem * rhs) const
{
	if (lhs->getRow() < rhs->getRow())
	{
		return true;
	}
	else if (lhs->getRow() == rhs->getRow())
	{
		return (lhs->getCol() < rhs->getCol());
	}
	else 
	{
		return false;
	}
}
