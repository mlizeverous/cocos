#include "HelloWorldScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);


	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	
	//倒计时
	time = Label::createWithTTF(ttfConfig, "180");
	//倒计时周期性调用调度器
	schedule(schedule_selector(HelloWorld::updateTime), 1.0f, kRepeatForever, 0);
	//倒计时的数字
	dtime = 180;
	time->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - time->getContentSize().height));
	addChild(time);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);

	//静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	//攻击动画
	//Vector<SpriteFrame*> attack;
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}
	
	//死亡动画
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	
	//运动动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}

	//Label
	auto menuLabel1 = Label::createWithTTF(ttfConfig, "W");
	auto menuLabel2 = Label::createWithTTF(ttfConfig, "S");
	auto menuLabel3 = Label::createWithTTF(ttfConfig, "A");
	auto menuLabel4 = Label::createWithTTF(ttfConfig, "D");
	auto menuLabel5 = Label::createWithTTF(ttfConfig, "X");
	auto menuLabel6 = Label::createWithTTF(ttfConfig, "Y");
	//menuItem
	auto item1 = MenuItemLabel::create(menuLabel1, CC_CALLBACK_1(HelloWorld::moveEvent, this,'W'));
	auto item2 = MenuItemLabel::create(menuLabel2, CC_CALLBACK_1(HelloWorld::moveEvent, this,'S'));
	auto item3 = MenuItemLabel::create(menuLabel3, CC_CALLBACK_1(HelloWorld::moveEvent, this,'A'));
	auto item4 = MenuItemLabel::create(menuLabel4, CC_CALLBACK_1(HelloWorld::moveEvent, this,'D'));
	auto item5 = MenuItemLabel::create(menuLabel5, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'X'));
	auto item6 = MenuItemLabel::create(menuLabel6, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'Y'));

	item3->setPosition(Vec2(origin.x+item3->getContentSize().width,origin.y+item3->getContentSize().height));
	item4->setPosition(Vec2(item3->getPosition().x + 3 * item4->getContentSize().width, item3->getPosition().y));
	item2->setPosition(Vec2(item3->getPosition().x + 1.5*item2->getContentSize().width, item3->getPosition().y));
	item1->setPosition(Vec2(item2->getPosition().x, item2->getPosition().y + item1->getContentSize().height));
	item5->setPosition(Vec2(origin.x+visibleSize.width-item5->getContentSize().width,item1->getPosition().y));
	item6->setPosition(Vec2(item5->getPosition().x-item6->getContentSize().width,item3->getPosition().y));
	
	auto menu = Menu::create(item1, item2, item3, item4, item5, item6, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);

	return true;
}


void HelloWorld::moveEvent(Ref*,char cid) {
	auto animation = Animation::createWithSpriteFrames(run, 0.1f);
	auto animate = Animate::create(animation);
	MoveBy* move = NULL;
	Spawn* action = NULL;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto sizex = visibleSize.width + origin.x;
	auto sizey = visibleSize.height + origin.y;
	double my_x = 0;
	double my_y = 0;
	switch (cid) {
	case 'W':
		if (pT->getPercentage() == 0 || dtime <= 0) break;
		my_y = player->getPosition().y;
		if ((my_y + 40) >= sizey) break;
		move = MoveBy::create(0.7, Point(0, 20));
		action = Spawn::createWithTwoActions(animate, move);
		player->runAction(action);
		break;
	case 'A':
		if (pT->getPercentage() == 0 || dtime <= 0) break;
		my_x = player->getPosition().x;
		if (my_x <= (origin.x + 35)) break;
		move = MoveBy::create(0.7, Point(-20, 0));
		action = Spawn::createWithTwoActions(animate, move);
		player->runAction(action);
		break;
	case 'S':
		if (pT->getPercentage() == 0 || dtime <= 0) break;
		my_y = player->getPosition().y;
		if (my_y <= (origin.y + 40)) break;
		move = MoveBy::create(0.7, Point(0,-20));
		action = Spawn::createWithTwoActions(animate, move);
		player->runAction(action);
		break;
	case 'D':
		if (pT->getPercentage() == 0 || dtime <= 0) break;
		my_x = player->getPosition().x;
		if ((my_x + 30) >= sizex) break;
		move = MoveBy::create(0.7, Point(20, 0));
		action = Spawn::createWithTwoActions(animate, move);
		player->runAction(action);
		break;
	}
}


void HelloWorld::actionEvent(Ref*, char cid) {
	Animation* animation;
	Animate* animate;
	auto stop = Animation::createWithSpriteFrames(idle, 0.1f);
	auto stopp = Animate::create(stop);
	Sequence* action = NULL;
	int temp = 0;
	
	//auto movement = Sequence::create()
	switch (cid) {
	case 'X':
		temp = pT->getPercentage();
		if (temp == 0 || dtime <= 0) break;
		else if (temp == 20) {
			animation = Animation::createWithSpriteFrames(dead, 0.1f);
			animate = Animate::create(animation);
			player->runAction(animate);
			pT->setPercentage(temp - 20);
			break;
		}
		else {
			animation = Animation::createWithSpriteFrames(dead, 0.1f);
			animate = Animate::create(animation);
			action = Sequence::createWithTwoActions(animate, stopp);
			player->runAction(action);
		
			pT->setPercentage(temp-20);
		}
		break;
	case 'Y':
		temp = pT->getPercentage();
		if (temp == 0 || dtime <= 0) break;
		animation = Animation::createWithSpriteFrames(attack, 0.1f);
		animate = Animate::create(animation);
		action = Sequence::createWithTwoActions(animate, stopp);
		player->runAction(action);
		
		pT->setPercentage(temp + 20);
		break;
	}
}

void HelloWorld::stopAc() {
	auto animation = Animation::createWithSpriteFrames(dead, 0.1f);
	auto animate = Animate::create(animation);
	player->runAction(animate);
	pT->setPercentage(0);
	return;
}

void HelloWorld::updateTime(float dt) {
	if (dtime == 0) {
		dtime--;
		stopAc();
		return;
	}
	if (dtime > 0) {
		dtime--;
		char string[10] = { 0 };
		sprintf(string, "%d", (int)dtime);
		time->setString(string);
	}
	return;
	
}