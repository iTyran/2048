//
//  HighScore.cpp
//  2048
//
//  Created by ZeroYang on 5/8/14.
//
//

#include "HighScore.h"

USING_NS_CC;

#define KHIGHSCORE "HighScore"

static HighScore* s_SharedHighScore = NULL;

HighScore* HighScore::getInstance()
{
    if (!s_SharedHighScore)
    {
        s_SharedHighScore = new HighScore();
        s_SharedHighScore->init();
    }
    
    return s_SharedHighScore;
}

void HighScore::destroyInstance()
{
    CC_SAFE_DELETE(s_SharedHighScore);
}

HighScore::HighScore()
{
    m_score = 0;
    m_highScore = 0;
}

HighScore::~HighScore()
{
    
}

bool HighScore::init()
{
    m_highScore = UserDefault::getInstance()->getIntegerForKey(KHIGHSCORE);
    
    return true;
}


int HighScore::getHighScore()
{
    return m_highScore;
}

int HighScore::getScore()
{
    return m_score;
}

void HighScore::setScore(int score)
{
    m_score = score;
    if (score > m_highScore) {
        m_highScore = score;
        UserDefault::getInstance()->setIntegerForKey(KHIGHSCORE, score);
    }
}


