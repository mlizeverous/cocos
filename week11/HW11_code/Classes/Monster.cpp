#include"Monster.h"
//#include"HelloWorldScene.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	Animation* anim = Animation::createWithSpriteFrames(monsterDead, 0.1f);
	Animate* ani = Animate::create(anim);
	Sequence* seq = ::Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	monster.eraseObject(sp);
}
void Factory::moveMonster(Vec2 playerPos,float time){
	for (auto it = monster.begin(); it != monster.end(); it++) {
		Vec2 m_pos = (*it)->getPosition();
		Vec2 dir = playerPos - m_pos;
		dir.normalize();
		(*it)->runAction(MoveBy::create(time, dir * 30));
	}
	//
}

Sprite* Factory::collider(Rect rect) {
	for (auto it = monster.begin(); it != monster.end(); it++) {
		if (rect.containsPoint((*it)->getPosition())) {
			return (*it);
		}
	}
	return NULL;
}