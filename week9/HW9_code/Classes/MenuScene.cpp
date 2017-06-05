#include "MenuScene.h"
#include "GameScene.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	Size mywinsize = Director::getInstance()->getWinSize();
	float winw = mywinsize.width; //获取屏幕宽度
	float winh = mywinsize.height;//获取屏幕高度
	float spx1 = bg_sky->getTextureRect().getMaxX();
	float spy1 = bg_sky->getTextureRect().getMaxY();
	bg_sky->setScaleX(winw / spx1); //设置精灵宽度缩放比例
	bg_sky->setScaleY(winh / spy1);
	this->addChild(bg_sky, 0);

	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	float spx = bg->getTextureRect().getMaxX();
	float spy = bg->getTextureRect().getMaxY();
	bg->setScaleX(winw / spx); //设置精灵宽度缩放比例
	bg->setScaleY(winh / spy);
	this->addChild(bg, 0);

	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);

	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);

	auto start = MenuItemImage::create("start-0.png",
									   "start-1.png",
										CC_CALLBACK_1(MenuSence::EnterGameScene, this));
	start->setPosition(Vec2(origin.x + 2 * (visibleSize.width / 3)+70, origin.y + visibleSize.height / 2 - 120));
	auto menu = Menu::create(start, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(Vec2(origin.x + 2 * (visibleSize.width / 3) + 60, origin.y + visibleSize.height / 2 - 170));
	this->addChild(gold, 1);

	auto text = Sprite::create("gold-miner-text.png");
	text->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 170));
	this->addChild(text, 1);

	return true;
}

void MenuSence::EnterGameScene(Ref * pSender)
{
	auto myScene = GameSence::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, myScene, Color3B(0, 0, 0)));
}


