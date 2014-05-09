//
//  GameScene.cpp
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#include "GameScene.h"
#include "Card.h"
#include "SimpleRecognizer.h"
#include "GameOverLayer.h"
#include "HighScore.h"
#include "PopLayer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::~GameScene()
{
    delete recognizer;
    
    HighScore::destroyInstance();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();
    
    //加入游戏背景
    auto layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(layerColorBG);
    

    //pause
    MenuItemFont::setFontName("Consolas");
    MenuItemFont::setFontSize(80);
    auto menuItemPause = MenuItemFont::create("PAUSE", CC_CALLBACK_1(GameScene::onPause, this));
    
    
    auto menu = Menu::create(menuItemPause, NULL);
    addChild(menu);
    menu->setPosition(Point(200, visibleSize.height - 200));
    
    //创建分数
    auto cardNumberTitle = Label::createWithSystemFont("SCORE","Consolas",80);
    cardNumberTitle->setPosition(Point(200, visibleSize.height - 100));
    addChild(cardNumberTitle);
    
    score = 0;
    cardNumberTTF = Label::createWithSystemFont("0", "Consolas", 70);
    cardNumberTTF->setPosition(Point(400, visibleSize.height - 100));
    addChild(cardNumberTTF);
    
    //设置触摸事件监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //创建4X4卡片
    createCardSprite(visibleSize);
    if (UserDefault::getInstance()->getBoolForKey("history")) {
        resumeStatus();
    }
    else
    {
        //初始时生成两个2
        createCardNumber(false);
        createCardNumber(false);
    }
    
    recognizer = new SimpleRecognizer();
    
    return true;
}

void GameScene::onPause(Ref* pSender)
{
    PopLayer *poplayer = PopLayer::create(Color4B(0,0,0,180));
    getParent()->addChild(poplayer);
    Director::getInstance()->pause();
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	Point beginPoint = touch->getLocation();
    recognizer->beginPoint(beginPoint);
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    Point pos = touch->getLocation();
    recognizer->movePoint(pos);
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    Point pos = touch->getLocation();
    SimpleGestures rtn = recognizer->endPoint(pos);
    
    switch (rtn) {
        case SimpleGesturesLeft:
            doLeft();
            doCheck();
            setScore(score);
            break;
        case SimpleGesturesRight:
            doRight();
            doCheck();
            setScore(score);
            break;
        case SimpleGesturesUp:
            doUp();
            doCheck();
            setScore(score);
            break;
        case SimpleGesturesDown:
            doDown();
            doCheck();
            setScore(score);
            break;
            
        case SimpleGesturesNotSupport:
        case SimpleGesturesError:
            log("not support or error touch,use geometricRecognizer!!");
            break;
            
        default:
            break;
    }
}

//根据屏幕大小创建卡片
void GameScene::createCardSprite(Size size)
{
    //求出单元格的宽和高
    //左右边距 cellSpace
    cellSize = (size.width - 3*cellSpace - 40)/4;
    
    
    //绘制出4X4的单元格
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //需要屏幕分辨率适配
            CardSprite *card = CardSprite::createCardSprite(0, cellSize, cellSize, getPosition(i, j));
            cardArr[i][j] = card;
            addChild(card);
        }
    }
}

//创建生成随机卡片
void GameScene::createCardNumber(bool animation)
{

    while (1) {
        int i = CCRANDOM_0_1() * 4;        //生成0~3随机数
        int j = CCRANDOM_0_1() * 4;
        
        log("[%d][%d]",i,j);
        
        if (cardArr[i][j]->getNumber() == 0)
        {
            //2和4的生成率为9:1
            cardArr[i][j]->setNumber(CCRANDOM_0_1()*10 < 1 ? 4 : 2);
            if(animation)
            {
                cardArr[i][j]->runNewNumberAction();
            }
            break;
        }
        
        if (!shouldCreateCardNumber()) {
            break;
        }
    }
}

bool GameScene::shouldCreateCardNumber()
{
    bool isCreate = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if( 0 == cardArr[i][j]->getNumber() )
            {
                isCreate = true;
                break;
            }
        }
    }
    
    return isCreate;
}

bool GameScene::isWin()
{
    bool win = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if( 2048 == cardArr[i][j]->getNumber() )
            {
                win = true;
                break;
            }
        }
    }
    
    return win;
}

//左滑动
bool GameScene::doLeft()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int x1 = x+1; x1<4; x1++)
            {
                if (cardArr[x1][y]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x--;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    
    return isMove;
}

//右滑动
bool GameScene::doRight()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 3; x >= 0; x--)
        {
            for (int x1 = x-1; x1>=0; x1--)
            {
                if (cardArr[x1][y]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x++;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    
    return isMove;
}

//上滑动
bool GameScene::doUp()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 3; y >= 0; y--)
        {
            for (int y1 = y-1; y1>=0; y1--)
            {
                if (cardArr[x][y1]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y++;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    
    return isMove;
}

//下滑动
bool GameScene::doDown()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y <4; y++)
        {
            for (int y1 = y+1; y1<4; y1++)
            {
                if (cardArr[x][y1]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y--;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    
    return isMove;
}

void GameScene::setScore(int score)
{
    cardNumberTTF->setString(__String::createWithFormat("%i",score)->getCString());
}

void GameScene::doCheck()
{
    bool isGameOver = true;

    //结束边界  4*4的card数值>0 且  相邻card没有相同数值
    //4*4的card数值>0 不能在创建Number
    //判断每一个的上下左右和自己是否相同
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (cardArr[x][y]->getNumber() == 0 ||
                (x<3 && cardArr[x][y]->getNumber() == cardArr[x+1][y]->getNumber()) ||
                (x>0 && cardArr[x][y]->getNumber() == cardArr[x-1][y]->getNumber()) ||
                (y<3 && cardArr[x][y]->getNumber() == cardArr[x][y+1]->getNumber()) ||
                (y>0 && cardArr[x][y]->getNumber() == cardArr[x][y-1]->getNumber()) )
            {
                isGameOver = false;
            }
        }
    }

    if (isWin()) {
        
        successLayer = LayerColor::create(Color4B(0, 0, 0, 180));
        Size winSize = Director::getInstance()->getWinSize();
        Point centerPos = Point(winSize.width / 2, winSize.height / 2);
        auto gameOverTitle = Label::createWithSystemFont("YOU WIN","Consolas",80);
        gameOverTitle->setPosition(centerPos);
        successLayer->addChild(gameOverTitle);
        
        getParent()->addChild(successLayer,1);
        
        scheduleOnce(SEL_SCHEDULE(&GameScene::removeSuccessLayer), 2);
        return;
    }
    
    //isGameOver = true;
    if (isGameOver)
    {
        log("game over");
        UserDefault::getInstance()->setBoolForKey("history", false);

        HighScore::getInstance()->setScore(score);
        GameOverLayer *gameoverLayer = GameOverLayer::create(Color4B(0, 0, 0, 180));
        getParent()->addChild(gameoverLayer,1);
        
        Director::getInstance()->pause();
    }
    else
    {
        if (shouldCreateCardNumber()) {
            createCardNumber();
            
            saveStatus();
        }
    }
    
}

Point GameScene::getPosition(int i, int j)
{
    float pX = 20 + cellSpace/2 + i*(cellSize+cellSpace);
    float pY = cellSize/2 + j*(cellSize+cellSpace);
    
    return Point(pX,pY);
}


void GameScene::saveStatus()
{
    char temp[10];
    //4*4
    for (int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++)
        {
            sprintf(temp,"%d%d",i,j);
            UserDefault::getInstance()->setIntegerForKey(temp, cardArr[i][j]->getNumber());
        }
    }
    
    UserDefault::getInstance()->setIntegerForKey("score", score);
    
    UserDefault::getInstance()->setBoolForKey("history", true);
}

void GameScene::resumeStatus()
{
    char temp[10];
    //4*4
    for (int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++)
        {
            sprintf(temp,"%d%d",i,j);
            int number = UserDefault::getInstance()->getIntegerForKey(temp);
            cardArr[i][j]->setNumber(number);
        }
    }
    
    score = UserDefault::getInstance()->getIntegerForKey("score");
    setScore(score);
    
    UserDefault::getInstance()->setBoolForKey("history", false);
}

void GameScene::removeSuccessLayer()
{
    successLayer->removeFromParent();
}