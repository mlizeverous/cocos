#include "Thunder.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Thunder::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool Thunder::init() {
    if ( !Layer::init() ) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    auto bgsprite = Sprite::create("background.jpg");
    bgsprite->setPosition(visibleSize / 2);
    // bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, \
    //     visibleSize.height / bgsprite->getContentSize().height);
    this->addChild(bgsprite, 0);

    player = Sprite::create("player.png");
    player->setPosition(visibleSize.width / 2, player->getContentSize().height + 5);
    player->setName("player");
    this->addChild(player, 1);

    addEnemy(5);

    preloadMusic();
    playBgm();

    addTouchListener();
    addKeyboardListener();
    addCustomListener();

    // TODO
    // add schedule
	schedule(schedule_selector(Thunder::update), 0.1f, kRepeatForever, 0);
    
    return true;
}

void Thunder::preloadMusic() {
    // TODO
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("/music/bgm.mp3");
	audio->preloadEffect("/music/explore.wav");
	audio->preloadEffect("/music/fire.wav");
}

void Thunder::playBgm() {
    // TODO
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("/music/bgm.mp3", true);
}

void Thunder::addEnemy(int n) {
    enemys.resize(n * 3);
    for(unsigned i = 0; i < 3; ++i) {
        char enemyPath[20];
        sprintf(enemyPath, "stone%d.png", 3 - i);
        double width  = (visibleSize.width - 20) / (n + 1.0),
               height = visibleSize.height - (50 * (i + 1));
        for(int j = 0; j < n; ++j) {
            auto enemy = Sprite::create(enemyPath);
            enemy->setAnchorPoint(Vec2(0.5, 0.5));
            enemy->setScale(0.5, 0.5);
            enemy->setPosition(width * (j + 1), height);
            enemys[i * n + j] = enemy;
			addChild(enemy);
        }
    }
}

void Thunder::addTouchListener(){
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // 
}

void Thunder::addKeyboardListener() {
    // TODO
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    // 
}

void Thunder::update(float f) {
    // TODO
	player->setPosition(player->getPosition() + Vec2(move, 0));
	//int total = 0;
	static double count = 0;
	static int dir = 1;
	count += f;
	if (count > 1) { count = 0.0; dir = -dir; }

	if (!bullet.empty()) {
		for (unsigned i = 0; i < bullet.size(); i++) {
			if (bullet[i] != NULL) {
				bullet[i]->setPosition(bullet[i]->getPositionX(), bullet[i]->getPositionY() + 5);
				if (bullet[i]->getPositionY() > visibleSize.height - 10) {
					bullet[i]->removeFromParentAndCleanup(true);
					bullet[i] = NULL;
				}
			}
			
		}
	}

	for (unsigned i = 0; i < enemys.size(); i++) {
		if (enemys[i] != NULL) {
			enemys[i]->setPosition(enemys[i]->getPosition() + Vec2(dir, -0.5));
			for (unsigned j = 0; j < bullet.size(); j++) {
				if (bullet[j] != NULL && bullet[j]->getPosition().getDistance(enemys[i]->getPosition()) < 30) {
					total--;
					index a;
					a._i = i;
					a._j = j;
					EventCustom e("meet");
					e.setUserData(static_cast<void*>(&a));
					_eventDispatcher->dispatchEvent(&e);
					if (total == 0) {
						auto label = Label::createWithTTF("YOU WIN", "fonts/Marker Felt.ttf", 64);
						Vec2 origin = Director::getInstance()->getVisibleOrigin();
						label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
						addChild(label, 3);
					}
					break;
				}
			}
		}
    }
	for (unsigned i = 0; i < enemys.size(); i++) {
		if (enemys[i] != NULL && player->getPosition().getDistance(enemys[i]->getPosition()) < 30) {
			auto fadeout = FadeOut::create(1.0f);
			player->runAction(fadeout);
			//removeFromParentAndCleanup(player);
			//player = NULL;
			auto label = Label::createWithTTF("YOU LOSE", "fonts/Marker Felt.ttf", 64);
			//Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();
			label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
			addChild(label, 3);
			break;
		}
	}
}

void Thunder::fire() {
    auto bullett = Sprite::create("bullet.png");
    bullett->setPosition(player->getPosition());
	bullet.push_back(bullett);
	addChild(bullett);
	auto audio = SimpleAudioEngine::getInstance();
	audio->pauseBackgroundMusic();
	audio->playEffect("/music/fire.wav");
	audio->resumeBackgroundMusic();
}

void Thunder::meet(EventCustom * event) {
	auto audio = SimpleAudioEngine::getInstance();
	audio->pauseBackgroundMusic();
	audio->playEffect("/music/explore.wav");
	audio->resumeBackgroundMusic();
	index* temp = static_cast<index*>(event->getUserData());
	int i = (*temp)._i;
	int j = (*temp)._j;
	enemys[i]->setVisible(false);
	enemys[i]->removeFromParentAndCleanup(true);
	enemys[i] = NULL;
	bullet[j]->setVisible(false);
	bullet[j]->removeFromParentAndCleanup(true);
	bullet[j] = NULL;
}

void Thunder::addCustomListener() {
    // TODO
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	_eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

bool Thunder::onTouchBegan(Touch *touch, Event *unused_event) {
    //
	auto location = touch->getLocation();
	if (player->getPosition().getDistance(location) < 30) {
		return true;
	}
	return false;
}

void Thunder::onTouchMoved(Touch *touch, Event *unused_event) {
	player->setPosition(touch->getLocation());
}

void Thunder::onTouchEnded(Touch *touch, Event *unused_event) {
	fire();
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code) {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
			player->setPosition(player->getPositionX() - 1, player->getPositionY());
			move -= 5;
            // TODO
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
			player->setPosition(player->getPositionX() - 1, player->getPositionY());
			move += 5;
            // TODO
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
            fire();
            break;
        default:
            break;
    }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		move = 0;
		break;
	default:
		break;
	}
}
