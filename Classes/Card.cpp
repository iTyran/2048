//
//  Card.cpp
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#include "Card.h"

USING_NS_CC;

CardSprite* CardSprite::createCardSprite(int number, int wight, int height, Point position)
{
	//这里是创建一个自定义精灵类，格式很规范化
	CardSprite *card = new CardSprite();
	if (card && card->init())
	{
		card->autorelease();
		card->initCard(number,wight,height,position);
		return card;
	}
	CC_SAFE_DELETE(card);
    
	return NULL;
}

CardSprite::~CardSprite()
{
    
}

//这里也是规范化
bool CardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void CardSprite::runNewNumberAction()
{
    auto action = ScaleBy::create(0.05,0.99);
    runAction(Sequence::create(action, action->reverse(), nullptr));
}

//获取数据
int CardSprite::getNumber()
{
	return number;
}

//设置数据
void CardSprite::setNumber(int num)
{
	number = num;
	//更新显示的数字
	if (num > 0)
	{
		labelCardNumber->setString(__String::createWithFormat("%i",number)->getCString());
	}
	else
	{
		labelCardNumber->setString("");
	}
    
	//设置数字大小
	if (num >= 0)
	{
		labelCardNumber->setSystemFontSize(100);
	}
	if (num >= 16)
	{
		labelCardNumber->setSystemFontSize(90);
	}
	if (num >= 128)
	{
		labelCardNumber->setSystemFontSize(60);
	}
	if (num >= 1024)
	{
		labelCardNumber->setSystemFontSize(40);
	}
    
	//判断数字的大小来调整颜色
	if(number == 0){
		colorBackground->setColor(Color3B(200,190,180));
	}
	if (number == 2) {
		colorBackground->setColor(Color3B(240,230,220));
	}
	if (number == 4) {
		colorBackground->setColor(Color3B(240,220,200));
	}
	if (number == 8) {
		colorBackground->setColor(Color3B(240,180,120));
	}
	if (number == 16) {
		colorBackground->setColor(Color3B(240,140,90));
	}
	if (number == 32) {
		colorBackground->setColor(Color3B(240,120,90));
	}
	if (number == 64) {
		colorBackground->setColor(Color3B(240,90,60));
	}
	if (number == 128) {
		colorBackground->setColor(Color3B(240,90,60));
	}
	if (number == 256) {
		colorBackground->setColor(Color3B(240,200,70));
	}
	if (number == 512) {
		colorBackground->setColor(Color3B(240,200,70));
	}
	if (number == 1024) {
		colorBackground->setColor(Color3B(0,130,0));
	}
	if (number == 2048) {
		colorBackground->setColor(Color3B(0,130,0));
	}
}

//初始化
void CardSprite::initCard(int number, int wight, int height, Point position)
{
	//初始化数字
	this->number = number;
	//加入卡片背景颜色
	colorBackground = LayerColor::create(Color4B(200,190,180,255),wight-15,height-15);
	colorBackground->setPosition(position);
    
	//判断如果大于0就显示，否则就不显示
	if (number > 0)
	{
		//加入字体
		labelCardNumber = Label::createWithSystemFont(__String::createWithFormat("%i",number)->getCString(),"Consolas",100);
		labelCardNumber->setPosition(Point(colorBackground->getContentSize().width/2, colorBackground->getContentSize().height/2));
		labelCardNumber->setTag(8);
		colorBackground->addChild(labelCardNumber);
	}
	else
	{
		//加入字体
		labelCardNumber = Label::createWithSystemFont("","Consolas",100);
		labelCardNumber->setPosition(Point(colorBackground->getContentSize().width/2, colorBackground->getContentSize().height/2));
		labelCardNumber->setTag(8);
		colorBackground->addChild(labelCardNumber);
	}
    
	this->addChild(colorBackground);
}