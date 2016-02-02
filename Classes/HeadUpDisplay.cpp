/*
 * HUD.cpp
 *
 *  Created on: 19/12/2013
 *      Author: Marco
 */

#include "HeadUpDisplay.h"
USING_NS_CC;

bool HeadUpDisplay::init(){

	if(!CCMenu::init())
		return false;

	/* Adding a "close" button to exit. */
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
			"CloseSelected.png", this, menu_selector(HeadUpDisplay::menuCloseCallback));

	CCSize scrSize = CCDirector::sharedDirector()->getWinSize();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	pCloseItem->setPosition(
					ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
						origin.y + pCloseItem->getContentSize().height/2));
	this->addChild(pCloseItem,1);


	/* Mesh */
	CCSprite* mesh = CCSprite::create();
	mesh->setTextureRect(CCRect(0,0,scrSize.width,scrSize.height));
	mesh->setColor(ccc3(0,0,0));
	mesh->setOpacity(0);
	mesh->setAnchorPoint(ccp(0,0));
	mesh->setPosition(CCPointZero);
	mesh->setTag(_meshTag);
	this->addChild(mesh,0);


	/* Bubble Talk */
	CCSprite* bubble = CCSprite::create("bubbleTalk.png");
	CCPoint bblePos = scrSize/2;
	bubble->setPosition(bblePos);
	bubble->setScale(0);
	bubble->setTag(_bubbleTalkTag);
	this->addChild(bubble,0);


	/* Message */
	CCLabelTTF* labelMsg = CCLabelTTF::create("Bonjour!\n :)","Artial", 32);
	CCSize bubbleSize = bubble->getContentSize();
	labelMsg->setPosition(bubbleSize/2);
	labelMsg->setOpacity(0);
	labelMsg->setTag(_messageTag);
	bubble->addChild(labelMsg,0);


	/* Accept Button */
	CCMenuItemImage* acceptButton = CCMenuItemImage::create(
			"acceptButton.png","acceptButtonPressed.png", this, menu_selector(HeadUpDisplay::hideMessage));
	acceptButton->retain();
	CCSize aBtnSize = acceptButton->getContentSize();
	acceptButton->setPosition(ccp(
			bblePos.x + bubbleSize.width/2  - aBtnSize.width  * .25f,
			bblePos.y - bubbleSize.height/2 + aBtnSize.height * .25f));
	acceptButton->setVisible(false);
	this->addChild(acceptButton,0,_acceptButtonTag);

	this->setPosition(CCPointZero);

	return true;
}

void HeadUpDisplay::showMessage(){

	/* A visible MenuItem is enabled as a button, so in order to no repeat
	 * the action of this method, we check if the accept button was enabled. */
	CCNode* acceptButton = this->getChildByTag(_acceptButtonTag);
	if(acceptButton->isVisible())
		return;


	/* Applying animation to show message elements.  */
	acceptButton->setVisible(true);

	CCNode* mesh = this->getChildByTag(_meshTag);
	mesh->stopAllActions();
	mesh->runAction(CCFadeTo::create(.25f, 255 * .5f));

	CCNode* bubbleTalk = this->getChildByTag(_bubbleTalkTag);
	bubbleTalk->stopAllActions();
	bubbleTalk->runAction(
			CCSequence::create(
				CCScaleTo::create(.25f, 0.75f, 1.25f),
				CCScaleTo::create(.25f, 1.00f, 1.00f),
				NULL
			)
	);

	CCNode* message = bubbleTalk->getChildByTag(_messageTag);
	message->stopAllActions();
	message->runAction(CCFadeTo::create(.5f, 255));
}

void HeadUpDisplay::hideMessage(){

	/* Applying animation to hide message elements.  */
	CCNode* acceptButton = this->getChildByTag(_acceptButtonTag);
	acceptButton->setVisible(false);

	CCNode* mesh = this->getChildByTag(_meshTag);
	mesh->stopAllActions();
	mesh->runAction(CCFadeTo::create(.25f, 0));

	CCNode* bubbleTalk = this->getChildByTag(_bubbleTalkTag);
	bubbleTalk->stopAllActions();
	bubbleTalk->runAction(CCScaleTo::create(.5f, 1.50f, 0.0f));

	CCNode* message = bubbleTalk->getChildByTag(_messageTag);
	message->stopAllActions();
	message->runAction(CCFadeTo::create(.5f, 0));
}

void HeadUpDisplay::menuCloseCallback(CCObject* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}
