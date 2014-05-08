//
//  HighScore.h
//  2048
//
//  Created by ZeroYang on 5/8/14.
//
//

#ifndef ___048__HighScore__
#define ___048__HighScore__

#include "cocos2d.h"

class HighScore
{
public:
    static HighScore* getInstance();
    static void destroyInstance();

    HighScore();
    ~HighScore();
    
    int getHighScore();
    int getScore();
    void setScore(int score);
 
private:
    bool init();
    int m_score;
    int m_highScore;
};

#endif /* defined(___048__HighScore__) */
