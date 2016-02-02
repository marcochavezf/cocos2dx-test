#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;


class GameLayer : public cocos2d::CCLayer{

private:
	CCArray* _poolClouds;
	CCSprite* _spriteMap;
	CCPoint _prevMapPos;
	float _prevDistancBtwnTouches;


	CCPoint validateMapLimitPos(CCPoint pos);
	void scrollMapTouchEnded();
	void zoom(float scaleFactor);
	void moveCloud(CCObject* pObj);
	void updateCloudsOpacity();

public:
	virtual ~GameLayer();
    virtual bool init();  
    
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
    CREATE_FUNC(GameLayer);
};


class GameScene : public CCScene {

private:
	GameLayer* _layer;

public:
	GameScene():_layer(NULL) {};
    ~GameScene();
    bool init();
    CREATE_FUNC(GameScene);
};

#endif // __GAME_LAYER_H__
