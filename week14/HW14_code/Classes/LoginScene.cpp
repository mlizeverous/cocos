#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}





// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(30);
    button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	button->addTouchEventListener(CC_CALLBACK_2(LoginScene::LoginClick, this));
    this->addChild(button, 2);

    return true;
}

void LoginScene::LoginClick(Ref *pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		string username = textField->getString();
		HttpRequest* request = new HttpRequest();
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("http://localhost:8080/login");
		request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
		string data = "username=" + username;
		const char* postData = data.data();
		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test");

		vector<string> headers;
		//headers.push_back("Cookies: GAMESESSIONID=9038a5b5fbfd35bc31365527e45448ae");
		headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
		request->setHeaders(headers);

		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
		Director::getInstance()->replaceScene(GameScene::createScene());
	}
}

void LoginScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char>*header = response->getResponseHeader();
	string _header = Global::toString(header);
	Global::gameSessionId = _header;
	std::vector<char>*buffer = response->getResponseData();
	string data = Global::toString(buffer);
	string message = "header: " + _header + "\n data: " + data;


}

