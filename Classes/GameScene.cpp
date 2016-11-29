#include "GameScene.h"

USING_NS_CC;

cocos2d::Scene * GameScene::createSceneWithRowsColors(int rows, int cols, int colors)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create(rows, cols, colors);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

GameScene * GameScene::create(int rows, int cols, int colors)
{
	GameScene * ret = new (std::nothrow) GameScene();
	if (ret && ret->init(rows, cols, colors))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool GameScene::init(int rows, int cols, int colors)
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_rows = rows;
	m_cols = cols;
	m_colors = colors;


	auto background = LayerColor::create(Color4B(22, 99, 96, 255));
	addChild(background, -1);
	


	auto button = ui::Button::create("btn-play-normal.png", "btn-play-selected.png");

	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->popScene();
			break;
		default:
			break;
		}
	});

	button->setPosition(Vec2(visibleSize.width + origin.x - button->getContentSize().width / 2, visibleSize.height + origin.y - button->getContentSize().height /2));

	addChild(button);

	auto button2 = ui::Button::create("btn-play-normal.png", "btn-play-selected.png");

	button2->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			lookForMatches();
			break;
		default:
			break;
		}
	});

	button2->setPosition(Vec2(visibleSize.width + origin.x - button->getContentSize().width *3/ 2, visibleSize.height + origin.y - button->getContentSize().height *3 / 2));

	addChild(button2);


	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);




	m_grid = std::vector<Vector<Gem*>>(m_cols, Vector<Gem*>(m_rows));
	

	fillGrid();

	while (true)
	{
		clearGrid();
		fillGrid();
		if(lookForMatches())
		{
			continue; 
		}

		if(!lookForPossibleMoves()) 
		{ 
			continue; 
		}
		break;
	}
	
	

	return true;
}

void GameScene::clearGrid()
{
	for (int col = 0; col<m_cols; ++col)
	{
		for (int row = 0; row < m_rows; ++row)
		{
			m_grid[col].at(row)->removeFromParent();
		}
		m_grid[col].clear();
	}
}

void GameScene::fillGrid()
{

	for (int col = 0; col < m_cols; ++col) 
	{
		while (m_grid[col].size()< m_rows)
		{
			addRandomGem(col);
		}
	}
}

void GameScene::addRandomGem(int col)
{
	auto gem = Gem::create(RandomHelper::random_int(0, m_colors));
	m_grid[col].pushBack(gem);
	int row = m_grid[col].size() - 1;
	gem->setPosition(Vec2(m_offset.x + col * m_stride, m_offset.y + row * m_stride));
	gem->setCol(col);
	gem->setRow(row);
	addChild(gem);
}

void GameScene::removeMatches()
{
	for (auto it = m_matched.rbegin();  it != m_matched.rend(); ++it) 
	{
		(*it)->removeFromParent();



	}
}

void GameScene::updateColumns()
{
}

bool GameScene::lookForMatches()
{
	m_matched.clear();

	for (int row = 0; row < m_rows; row++) {
		for (int col = 0; col < m_cols; col++) {
			auto match = getMatchHoriz(col, row);
			if (match.size() > 2) {
				for (auto& i : match)
				{
					m_matched.insert(i);
				}
				col += match.size() - 1;
			}
		}
	}
   
	for (int col = 0; col < m_cols; col++) {
		for (int row = 0; row < m_rows; row++) {
			auto match = getMatchVert(col, row);
			if (match.size() > 2) {
				for (auto& i : match)
				{
					m_matched.insert(i);
				}
				row += match.size() - 1;
			}
		}
	}

	return !m_matched.empty();
}

bool GameScene::lookForPossibleMoves()
{
	for (int col = 0; col < m_cols; col++) {
		for (int row = 0; row < m_rows; row++) {
     
			if (matchPattern(col, row, { 1,0 }, { {-2, 0},{-1, -1},{-1, 1},{2, -1},{2, 1},{3, 0} }))
			{
				return true;
			}
 
			if (matchPattern(col, row, { 2, 0 }, { {1, -1}, {1, 1} }))
			{
				return true;
			}

			if (matchPattern(col, row, { 0, 1 }, { {0, -2},{-1, -1},{1, -1},{-1, 2},{1, 2},{0, 3} }))
			{
				return true;
			}
     
			if (matchPattern(col, row, { 0, 2 }, { {-1, 1},{1, 1} }))
			{
				return true;
			}
		}
	}
  
	return false;
}

std::set<Gem*, GemComp> GameScene::getMatchHoriz(int col, int row)
{
	std::set<Gem*, GemComp> match = {m_grid[col].at(row)};
	for (int i = 1; col + i < m_cols; i++) {
		if (m_grid[col].at(row)->getType() == m_grid[col + i].at(row)->getType()) 
		{
			match.insert(m_grid[col + i].at(row));
		}
		else {
			return match;
		}
	}

	return match;
}

std::set<Gem*, GemComp> GameScene::getMatchVert(int col, int row)
{
	std::set<Gem*, GemComp> match = { m_grid[col].at(row) };
	for (int i = 1; row + i < m_rows; i++) {
		if (m_grid[col].at(row)->getType() == m_grid[col].at(row + i)->getType())
		{
			match.insert(m_grid[col].at(row + i));
		}
		else {
			return match;
		}
	}

	return match;
}

bool GameScene::matchPattern(int col, int row, std::vector<int> neighbour, std::vector<std::vector<int>> possible)
{
	if (!matchType(col + neighbour[0], row + neighbour[1], m_grid[col].at(row)->getType())) 
	{
		return false;
	}

	for (int i = 0; i < possible.size(); i++) {
		if (matchType(col + possible[i][0], row + possible[i][1], m_grid[col].at(row)->getType())) {
			return true;
		}
	}
	return false;
}

bool GameScene::matchType(int col, int row, int type)
{
	if ((col < 0) || (col > m_cols-1) || (row < 0) || (row > m_rows-1))
	{
		return false;
	}
	return (m_grid[col].at(row)->getType() == type);

}

void GameScene::makeSwap(Gem * first, Gem * second)
{
	m_userMoved = true;
	swapGems(first, second);

}

void GameScene::swapGems(Gem * first, Gem * second)
{
	auto moveToSecond = MoveTo::create(0.5, Vec2(m_offset.x + second->getCol() * m_stride, m_offset.y + second->getRow() * m_stride));
	first->runAction(moveToSecond);
	
	auto moveToFirst = MoveTo::create(0.5, Vec2(m_offset.x + first->getCol() * m_stride, m_offset.y + first->getRow() * m_stride));
	auto onEnd = CallFunc::create([=](){
		onSwapEnd(first, second);
	});
	auto seq = Sequence::create(moveToFirst, onEnd, nullptr);
	second->runAction(seq);

	m_grid[first->getCol()].replace(first->getRow(), second);
	m_grid[second->getCol()].replace(second->getRow(), first);

	auto tempcol = first->getCol();
	auto temprow = first->getRow();
	first->setRow(second->getRow());
	first->setCol(second->getCol());
	second->setRow(temprow);
	second->setCol(tempcol);

	
}

void GameScene::onSwapEnd(Gem* first, Gem* second)
{
	if (m_userMoved)
	{
		m_userMoved = false;
		if (!lookForMatches()) {
			swapGems(second, first);
		}
	}
}

void GameScene::onGemTouch(Gem * gem)
{
	if (m_first == nullptr) 
	{
		m_first = gem;
		gem->setScale(getScale()*1.5);
	}
	else if (m_first == gem)
	{
		m_first = nullptr;
		gem->setScale(1);
	}
	else
	{   
		m_first->setScale(1);
		if ((m_first->getRow() == gem->getRow()) && (abs(m_first->getCol() - gem->getCol()) == 1)) 
		{
			makeSwap(m_first, gem);
			m_first = nullptr;
 
		}
		else if ((m_first->getCol() == gem->getCol()) && (abs(m_first->getRow() - gem->getRow()) == 1)) 
		{
			makeSwap(m_first, gem);
			m_first = nullptr;
  
		}
		else 
		{
			m_first = gem;
			gem->setScale(getScale()*1.5);
		}
	}
}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	return true;
}

void GameScene::onTouchEnded(Touch * touch, Event * unused_event)
{
	auto location = this->convertTouchToNodeSpace(touch);
	for (auto col: m_grid)
	{
		for (auto gem : col)
		{
			if (gem->getBoundingBox().containsPoint(location))
			{
				onGemTouch(gem);
			}
		}

	}
}


const cocos2d::Vec2 GameScene::m_offset(100, 200);
