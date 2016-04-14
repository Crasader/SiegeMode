#include "pch.h"
#include "UnitTest.h"

bool UnitTest::init()
{
	/*
	* �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	*/
	if (!this->Unit::init())
		return false;


	this->initWithSpriteFrameName(settings::PATHNAME_SPRITE_SWORDMAN + "/swordman_walk_1.png");
	//this->setSpriteFromFileName("swordman_walk_1.png");
	this->getTexture()->setAliasTexParameters();
	this->setAnchorPoint(Vec2(21.f / 28.f, 3.f / 28.f));
	auto asdf = this->getAnchorPoint();
	this->setToAlias();
	return true;
}