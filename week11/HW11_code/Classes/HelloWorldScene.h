#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    

	void moveEvent(Ref*, char);
	void actionEvent(Ref*, char);
	void stopAc();
	void updateTime(float dt);
	void hitByMonster(float dt);
	void newmonster(float dt);
	void moveMonster(float dt);
	void updateHitMonsters();
	Sprite* attackMonster();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	//void update(float dt) override;
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Label* hitNumber;
	int dtime;
	int hitMonster;
	cocos2d::ProgressTimer* pT;
};

#endif // __HELLOWORLD_SCENE_H__
