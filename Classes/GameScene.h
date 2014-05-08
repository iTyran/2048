//
//  GameScene.h
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#ifndef ___048__GameScene__
#define ___048__GameScene__

#include "cocos2d.h"

class CardSprite;
class SimpleRecognizer;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(GameScene);
    ~GameScene();
    
    //触摸事件监听回调函数
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    
    //virtual void onEnter() override;
    
private:
    
    void createCardSprite(cocos2d::Size size);
    void createCardNumber();
    void newNumber(int i, int j, int num);
    void setScore(int score);
    void doCheck();
    bool shouldCreateCardNumber();
    
    void saveStatus();
    void resumeStatus();

    
    cocos2d::Point getPosition(int i, int j);
    
    /////////////////
    //左滑动
    bool doLeft();
    //右滑动
    bool doRight();
    //上滑动
    bool doUp();
    //下滑动
    bool doDown();

private:

    //定义cellSize/cellSpace
    int cellSize = 100;
    int cellSpace = 10;
    //分数
    int score;
    //显示分数控件
    cocos2d::Label *cardNumberTTF;
    
    //储存卡片类
    CardSprite *cardArr[4][4];

    SimpleRecognizer *recognizer;
};

#endif /* defined(___048__GameScene__) */
