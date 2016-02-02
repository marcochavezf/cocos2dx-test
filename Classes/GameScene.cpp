#include "GameScene.h"
#include "AppMacros.h"
#include "HeadUpDisplay.h"
USING_NS_CC;


bool GameScene::init(){
	if( CCScene::init() ){
		this->_layer = GameLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		return true;
	}else{
		return false;
	}
}

GameScene::~GameScene(){
	CC_SAFE_RELEASE(_layer);
}


GameLayer::~GameLayer(){
	CC_SAFE_RELEASE(_poolClouds);
}

// on "init" you need to initialize your instance
bool GameLayer::init() {

	if (!CCLayer::init())
		return false;

	/* Adding Head Up Display Menu */
	HeadUpDisplay* hudMenu = HeadUpDisplay::create();
	this->addChild(hudMenu, 1);


	/* Adding New York map. */
	_spriteMap = CCSprite::create("new_york_map.jpg");
	_spriteMap->setPosition(ccp(210,30));
	_spriteMap->setScale(.65f);
	this->addChild(_spriteMap, 0);


	/* Adding clouds  */
	_poolClouds = CCArray::create();
	_poolClouds->retain();
	for(int i=1; i<=2; i++){
		for(int j=1; j<=3; j++){
			CCString* nameImage = CCString::createWithFormat("cloud%i.png",i);
			CCSprite* cloud = CCSprite::create(nameImage->getCString());
			cloud->setScale(2);
			this->moveCloud(cloud);
			_poolClouds->addObject(cloud);
			_spriteMap->addChild(cloud);
		}
	}
	updateCloudsOpacity();

	/* Buttons that will show a message in the screen when they are pressed.
	 * This buttons are adding to a menu, which will be a map's child. */


	/* Star Button */
	CCMenuItemImage *star = CCMenuItemImage::create(
				"star.png",NULL, hudMenu, menu_selector(HeadUpDisplay::showMessage));
	star->setPosition(ccp(720,1150));

	CCAction* starAction = CCRepeatForever::create(
			CCSequence::create(
				CCDelayTime::create(3.f),
				CCEaseIn::create(CCMoveBy::create(.25f, ccp(0, +10)),2),
				CCRotateBy::create(.075f, +7.5f),
				CCRepeat::create(
					CCSequence::create(
						CCRotateBy::create(.15f, -15.f),
						CCRotateBy::create(.15f, +15.f),
					NULL),3
				),
				CCRotateBy::create(.075f, -7.5f),
				CCEaseElasticIn::create(CCMoveBy::create(.75f, ccp(0, -10)),2),
			NULL)
	);
	star->runAction(starAction);


	/* Arrow Button */
	CCMenuItemImage *arrow = CCMenuItemImage::create(
				"arrow.png",NULL, hudMenu, menu_selector(HeadUpDisplay::showMessage));
	float arrowHeight = arrow->getContentSize().height;
	arrow->setPosition(ccp(star->getPosition().x,
						   star->getPosition().y + arrowHeight/2));
	arrow->setAnchorPoint(ccp(.5f, 0));
	arrow->setOpacity(255 * .75f);
	CCAction* arrowActionOne = CCRepeatForever::create(
			CCJumpBy::create(1.5f, ccp(0, 0), 20, 1)
	);
	CCAction* arrowActionTwo = CCRepeatForever::create(
			CCSequence::create(
				CCEaseOut::create(CCScaleTo::create(.75f, 1.0f, 1.0f),2),
				CCEaseIn::create( CCScaleTo::create(.75f, .95f, .75f),2),
			NULL)
	);

	arrow->runAction(arrowActionOne);
	arrow->runAction(arrowActionTwo);

	/* Creating a menu to attach star and arrow buttons.
	 * This menu will be a map child, in order to move and
	 * scale it with the map. */
	CCMenu* mapMenu = CCMenu::create();
	mapMenu->setPosition(CCPointZero);
	mapMenu->addChild(star);
	mapMenu->addChild(arrow);
	_spriteMap->addChild(mapMenu);


	/* We store a map previous position, because we cannot obtain a
	 * precise position difference in "ccTouchesEnded" method (previous
	 * touch location and current touch location are always the same). */
	_prevMapPos = _spriteMap->getPosition();

	/* This member stores the previous scalar distance between two touches
	 * when we are panning the map. */
	_prevDistancBtwnTouches = 0.0f;

	this->setTouchEnabled(true);

	return true;
}


void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {

	_prevMapPos = _spriteMap->getPosition();

}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {

	CCTouch* touchOne = (CCTouch*) (touches->anyObject());
	CCPoint locationOne = touchOne->getLocation();

	/* Scrolling. */
	if (touches->count() == 1) {

		_prevMapPos = _spriteMap->getPosition();
		CCPoint prevLocation = touchOne->getPreviousLocation();

		/* Obtaining the difference between the current touch and previous touch. */
		float diffX = prevLocation.x - locationOne.x;
		float diffY = prevLocation.y - locationOne.y;

		CCSize scrSize = CCDirector::sharedDirector()->getWinSize();
		CCSize mapSize = _spriteMap->getContentSize() * _spriteMap->getScale();

		/* Limit scroll in X when the width map has the same size of width screen */
		if (mapSize.width <= scrSize.width)
			diffX = 0;

		/* We are applying the difference to obtain the new map position, and
		 * check if the new position is within screen, in opposite case, will be
		 * applied a friction to the new position (effect of hard dragging when the
		 * map position is outside of screen). */
		float newPosX = _spriteMap->getPosition().x - diffX;
		float newPosY = _spriteMap->getPosition().y - diffY;

		float frictionFactor = 75.f * _spriteMap->getScale();



		/* Check map width limit */
		float leftMapBorder  = newPosX - mapSize.width / 2;
		float rightMapBorder = newPosX + mapSize.width / 2;

		float leftWinLimit 	= 0;
		float rightWinLimit = scrSize.width;
		float newDiffX 		= 0;

		if (leftMapBorder > leftWinLimit)
			newDiffX = leftMapBorder / frictionFactor * diffX;
		else
		if (rightMapBorder < rightWinLimit)
			newDiffX = (rightWinLimit - rightMapBorder) / frictionFactor * diffX;

		newDiffX = abs(newDiffX) > abs(diffX) ? diffX : newDiffX;
		newPosX += newDiffX;



		/* Check map height limit */
		float topMapBorder = newPosY - mapSize.height / 2;
		float bottomMapBorder = newPosY + mapSize.height / 2;

		float topWinLimit = 0;
		float bottomWinLimit = scrSize.height;

		float newDiffY = 0;
		if (topMapBorder > topWinLimit)
			newDiffY = topMapBorder / frictionFactor * diffY;
		else
		if (bottomMapBorder < bottomWinLimit)
			newDiffY = (bottomWinLimit - bottomMapBorder) / frictionFactor* diffY;

		newDiffY = abs(newDiffY) > abs(diffY) ? diffY : newDiffY;
		newPosY += newDiffY;


		/* Set new position */
		_spriteMap->setPosition(CCPointMake(newPosX,newPosY));

	} else
	if (touches->count() == 2) { /* Panning. */

		/* Getting the difference between previous distance and current distance
		 * of the touches. Then that difference will be divided by screen width
		 * in order to get a scale factor and be applied to the map. */

		CCSetIterator iter 	= touches->begin();
		CCTouch* touchTwo 	= (CCTouch*)(*++iter);
		CCPoint locationTwo = touchTwo->getLocation();

		CCPoint vectorDistance = locationTwo - locationOne;
		float curDistancBtwnTouches = sqrt(pow(vectorDistance.x, 2) + pow(vectorDistance.y, 2));

		if (_prevDistancBtwnTouches != 0.0f) {
			float diffBtwnPrevCurTouches = _prevDistancBtwnTouches - curDistancBtwnTouches;
			float winWidth = CCDirector::sharedDirector()->getWinSize().width;
			float scaleFactor = diffBtwnPrevCurTouches / winWidth;

			zoom(scaleFactor);
		}
		_prevDistancBtwnTouches = curDistancBtwnTouches;
	}

}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	_prevDistancBtwnTouches = 0.0f;
	_spriteMap->stopAllActions();

	CCPoint newPos = validateMapLimitPos(_spriteMap->getPosition());
	CCPoint curPos = _spriteMap->getPosition();

	float duration = .3f;
	/* If the new position is equal from current position,
	 * then the map is scrolling within scene, otherwise it
	 * it is scrolling out.  */
	if (newPos.x == curPos.x && newPos.y == curPos.y) {

		/* If the map is scrolling within scene, then it will
		 * have a dragging effect when the user drop the touch.
		 *
		 * The speed of the effect, will be based on the previous
		 * map position. */

		float factorDiff = 3.5f;
		CCPoint diff = ccp((_prevMapPos.x - curPos.x) * factorDiff,
						   (_prevMapPos.y - curPos.y) * factorDiff);
		CCPoint newPosDrag = validateMapLimitPos(newPos - diff);

		CCFiniteTimeAction* actionMove = CCMoveTo::create(duration, newPosDrag);
		CCActionInterval* move_ease_out = CCEaseOut::create((CCActionInterval*) actionMove, 2.f);
		_spriteMap->runAction(move_ease_out);

	} else {

		/* If the map is scrolling outside of screen, then it will return to a valid position. */
		CCFiniteTimeAction* actionMove = CCMoveTo::create(duration, newPos);
		_spriteMap->runAction(actionMove);
	}
}


/**
 * This method check if the position given is valid
 * within screen limits, in opposite case it will be
 * set to a valid position.
 */
CCPoint GameLayer::validateMapLimitPos(CCPoint pos) {

	CCSize scrSize = CCDirector::sharedDirector()->getWinSize();
	CCSize mapSize = _spriteMap->getContentSize() * _spriteMap->getScale();

	/* Check width limit */
	if (pos.x - mapSize.width / 2 > 0)
		pos.x = mapSize.width / 2;
	else
	if (pos.x + mapSize.width / 2 < scrSize.width)
		pos.x = scrSize.width - mapSize.width / 2;

	/* Check height limit */
	if (pos.y - mapSize.height / 2 > 0)
		pos.y = mapSize.height / 2;
	else
	if (pos.y + mapSize.height / 2 < scrSize.height)
		pos.y = scrSize.height - mapSize.height / 2;

	return pos;
}

/**
 * This method scale the map according to scale factor give it
 * when the player pinch the screen. It checks if the new scale
 * of the map is not out of its bounds.
 */
void GameLayer::zoom(float scaleFactor) {

	float newScale = _spriteMap->getScale() - scaleFactor;
	newScale = newScale > 1.0f ? 1.0f : newScale;

	CCSize scrSize = CCDirector::sharedDirector()->getWinSize();
	CCSize mapSize = _spriteMap->getContentSize();

	if (mapSize.width * newScale < scrSize.width)
		newScale = scrSize.width / mapSize.width;

	if (newScale != _spriteMap->getScale())
		_spriteMap->setScale(newScale);

	updateCloudsOpacity();
}

/**
 * Move the cloud send it with an interval action. The position of the
 * cloud will be random and once it finish its movement, this method will
 * be called again.
 */
void GameLayer::moveCloud(CCObject* pSender){
	CCSprite* cloud = (CCSprite*)pSender;
	CCSize mapSize = _spriteMap->getContentSize();
	CCSize cldSize = cloud->getContentSize();

	int posY  = rand() % (int)(mapSize.height * .8f) + mapSize.height * .1f;
	float posX1 = -cldSize.width;
	float posX2 = mapSize.width + cldSize.width;
	float durat = 90.f;	//90 seconds
	CCActionInterval* move;
	CCPoint initPos;

	if(posY % 2 == 0){
		/* Move left to right. */
		initPos = ccp(posX1,posY);
		move = CCMoveTo::create(durat, ccp(posX2,posY));
	}else{
		/* Move right to left. */
		initPos = ccp(posX2,posY);
		move = CCMoveTo::create(durat, ccp(posX1,posY));
	}

	cloud->setPosition(initPos);
	cloud->runAction(CCSequence::create(
					move,
					CCCallFuncN::create(this,callfuncN_selector(GameLayer::moveCloud)),
					NULL));
}

/**
 * Update clouds' opacity according to scale of the map.
 * This method is called when the player scales the map (zoom).
 */
void GameLayer::updateCloudsOpacity(){

	CCObject* obj = NULL;
	CCARRAY_FOREACH(_poolClouds, obj){
		CCSprite *cloud = dynamic_cast<CCSprite*>(obj);

		float limitOpacity = 200;
		float opacity     = limitOpacity/_spriteMap->getScale() - limitOpacity;
		opacity = opacity > limitOpacity ? limitOpacity : opacity;
		opacity = opacity <   		   0 ?   		  0 : opacity;

		cloud->setOpacity(opacity);
	}
}
