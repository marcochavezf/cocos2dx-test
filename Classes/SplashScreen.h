/*
 * SplashScene.h
 *
 *  Created on: 21/12/2013
 *      Author: Marco
 */

#ifndef SPLASHSCENE_H_
#define SPLASHSCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class SplashScreen : public CCLayer {

private:
    CCLabelTTF *_labelLoading;
    CCLabelTTF *_labelPercent;
    int _numberOfSprites;
    int _numberOfLoadedSprites;
    void loadingCallBack(CCObject *obj);

public:
	virtual bool init();
	CREATE_FUNC(SplashScreen);
};

#endif /* SPLASHSCENE_H_ */
