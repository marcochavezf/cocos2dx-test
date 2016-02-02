/*
 * HUD.h
 *
 *  Created on: 19/12/2013
 *      Author: Marco
 */

#ifndef __HEAD_UP_DISPLAY_H__
#define __HEAD_UP_DISPLAY_H__

#include "cocos2d.h"
USING_NS_CC;


enum {
    _bubbleTalkTag,
    _acceptButtonTag,
    _messageTag,
    _meshTag
};

class HeadUpDisplay : public CCMenu {

public:
	virtual bool init();

	void showMessage();
	void hideMessage();
	void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(HeadUpDisplay);
};

#endif /* HUD_H_ */
