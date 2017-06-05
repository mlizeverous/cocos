#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //获取屏幕宽度
	float winh = mywinsize.height;//获取屏幕高度
	float spx = bg->getTextureRect().getMaxX();
	float spy = bg->getTextureRect().getMaxY();
	bg->setScaleX(winw / spx); //设置精灵宽度缩放比例
	bg->setScaleY(winh / spy);
	this->addChild(bg, 0);

	auto label = Label::createWithTTF("shoot", "fonts/Marker Felt.ttf", 64);
	auto shoot = MenuItemLabel::create(label);
	shoot->setCallback(CC_CALLBACK_1(GameSence::shootCallback, this));
	shoot->setPosition(700, 480);
	auto menu = Menu::create(shoot, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	mouseLayer = Layer::create();
	mouseLayer->setAnchorPoint(ccp(0, 0));
	mouseLayer->setPosition(0, winh / 2);
	this->addChild(mouseLayer);

	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition( origin.x + visibleSize.width/2 , 0);
	
	mouseLayer->addChild(mouse, 1);

	stoneLayer = Layer::create();
	stoneLayer->setAnchorPoint(ccp(0, 0));
	stoneLayer->setPosition(0, 0);
	this->addChild(stoneLayer);

	stone = Sprite::create("stone.png");
	stone->setPosition(560, 480);
	stoneLayer->addChild(stone, 1);

	return true;
}

void GameSence::shootCallback(Ref * pSender) { // shoot，发射石头
	auto location = mouse->getPosition();
	auto _location = mouseLayer->convertToWorldSpace(location);
	auto fadeout = FadeOut::create(3.0f);
	auto move = MoveTo::create(1, stoneLayer->convertToNodeSpace(_location));

	auto _stone = Sprite::create("stone.png");
	_stone->setPosition(stone->getPosition());
	this->addChild(_stone);

	auto shoot = Spawn::createWithTwoActions(fadeout, move);
	_stone->runAction(shoot);

	//Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	auto movee = MoveTo::create(2, Point(50, 0));

	//auto MouseToCheese = Spawn::createWithTwoActions(RepeatForever::create(mouseAnimate), movee);
	mouse->runAction(movee);


	auto diamond = Sprite::create("diamond.png");
	diamond->setPosition(mouseLayer->convertToWorldSpace(location));
	this->addChild(diamond, 1);
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();

	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	this->addChild(cheese, 1);
	auto fadeout = FadeOut::create(3.0f);
	cheese->runAction(fadeout);

	auto _location = mouseLayer->convertToNodeSpace(location);
	//Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	auto move = MoveTo::create(2, _location);
	//auto MouseToCheese = Spawn::createWithTwoActions(RepeatForever::create(mouseAnimate), move);
	mouse->runAction(move);


	return true;
}
//gem-mouse-0