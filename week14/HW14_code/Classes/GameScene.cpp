#include "GameScene.h"
#include "LoginScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <sstream>
#include <stdlib.h>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
	submit_button->addTouchEventListener(CC_CALLBACK_2(GameScene::SubmitClick, this));
    this->addChild(submit_button, 2);

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
	rank_button->addTouchEventListener(CC_CALLBACK_2(GameScene::RankClick, this));
    this->addChild(rank_button, 2);

    return true;
}

void GameScene::SubmitClick(Ref *pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		string score = score_field->getString();
		HttpRequest* request = new HttpRequest();
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("http://localhost:8080/submit");
		request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestScoreCompleted, this));
		string data = "score=" + score;
		const char* postData = data.data();
		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test");
		vector<string> headers;
		headers.push_back("Cookies: GAMESESSIONID=" + Global::getSessionIdFromHeader(Global::gameSessionId));
		headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
		request->setHeaders(headers);

		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
}

void GameScene::RankClick(Ref *pSender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		HttpRequest* request = new HttpRequest();
		std::stringstream ss;
		ss << rand();
		string s;
		ss >> s;
		request->setUrl(("http://localhost:8080/rank?top=10&rand=" + s).data());
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestRankCompleted, this));
		request->setTag("GET test");
		vector<string> headers;
		headers.push_back("Cookies: GAMESESSIONID=" + Global::getSessionIdFromHeader(Global::gameSessionId));
		headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
		request->setHeaders(headers);

		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
}

void GameScene::onHttpRequestScoreCompleted(HttpClient *sender, HttpResponse * response) {
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char>*header = response->getResponseHeader();
	string headerr = Global::toString(header);
	std::vector<char>*buffer = response->getResponseData();
	string data = Global::toString(buffer);
	string message = "header: " + headerr + "\n data: " + data;
}

void GameScene::onHttpRequestRankCompleted(HttpClient *sender, HttpResponse * response) {
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	vector<char>*buffer = response->getResponseData();
	string info = Global::toString(buffer);
	rapidjson::Document d;
	d.Parse<0>(info.data());
	if (d.HasParseError()) {
		CCLOG("GetParseError %s\n", d.GetParseError());
	}
	if (d.IsObject() && d.HasMember("info")) {
		string re = d["info"].GetString();
		string out = "";
		for (int i = 1; i < re.length(); i++) {
			if (re[i] == '|')
				out += "\n";
			else out += re[i];
		}
		rank_field->setString(out);
	}

}
