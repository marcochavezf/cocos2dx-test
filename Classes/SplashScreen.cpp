/*
 * SplashScene.cpp
 *
 *  Created on: 21/12/2013
 *      Author: Marco
 */

#include "SplashScreen.h"
#include "GameScene.h"

bool SplashScreen::init(){

	if (!CCLayer::init())
		return false;

	_numberOfLoadedSprites = 0;
	_numberOfSprites 	   = 7;

    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCSprite* bg = CCSprite::create();
    bg->setTextureRect(CCRect(0,0,size.width,size.height));
    bg->setColor(ccc3(0,0,0));
    bg->setAnchorPoint(ccp(0,0));

    _labelLoading = CCLabelTTF::create("Loading Pretty Simple Coding Test\nDeveloper: Marco Chavez", "Arial", 15);
    _labelPercent = CCLabelTTF::create("%0", "Arial", 15);

    _labelLoading->setPosition(ccp(size.width / 2, size.height / 2 + 20));
    _labelPercent->setPosition(ccp(size.width / 2, size.height / 2 - 25));

    this->addChild(bg);
    this->addChild(_labelLoading);
    this->addChild(_labelPercent);

    /* Loading images */
    CCTextureCache::sharedTextureCache()->addImageAsync("acceptButton.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("acceptButtonPressed.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("arrow.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("star.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("cloud1.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("cloud2.png", this, callfuncO_selector(SplashScreen::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync("new_york_map.jpg", this, callfuncO_selector(SplashScreen::loadingCallBack));

    return true;
}

void SplashScreen::loadingCallBack(CCObject *obj){


    ++_numberOfLoadedSprites;
    char tmp[10];
    sprintf(tmp,"%%%d", (int)(((float)_numberOfLoadedSprites / _numberOfSprites) * 100));
    _labelPercent->setString(tmp);

    CCDirector::sharedDirector()->setDepthTest(false);

    if (_numberOfLoadedSprites == _numberOfSprites){

    	sprintf(tmp,"Enjoy! :)");
    	_labelLoading->setString(tmp);

    	 GameScene* mainGame = GameScene::create();
    	 CCScene* transitionScene = CCTransitionCrossFade::create(1.25f, mainGame);
    	 CCDirector::sharedDirector()->replaceScene(transitionScene);
    }
}
