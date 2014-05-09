//
//  Card.h
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#ifndef ___048__Card__
#define ___048__Card__

#include "cocos2d.h"

class CardSprite : public cocos2d::Sprite
{
public:
    static CardSprite* createCardSprite(int number, int wight, int height,cocos2d::Point position);
	virtual bool init();
	CREATE_FUNC(CardSprite);
    ~CardSprite();
    int getNumber();
    void setNumber(int num);
    void runNewNumberAction();
private:
    void initCard(int number, int wight, int height, cocos2d::Point position);
private:
    //显示数字
    int number;
    
    //Label控件
    cocos2d::Label *labelCardNumber;
    
    //背景
    cocos2d::LayerColor *colorBackground;
};

#endif /* defined(___048__Card__) */
