#include "pch.h"
#include "Unit.h"

bool Unit::init()
{
	/*
	* �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	*/
	if (!this->Actor::init())
		return false;

	_cursor = Actor::create();
	_cursor->initWithSpriteFrameName("unitSelector1.png");
	this->addChild(_cursor);
	_cursor->setPosition(this->getContentSize());
	_cursor->setVisible(false);

	return true;
}

bool Unit::touchCallback(Touch* touch, Event* event)
{
	if (this->Actor::touchCallback(touch, event))
	{
		if (_selected)
		{
			_cursor->setVisible(false);
			_selected = false;
			return true;
		}
		//ó�� �����ϴ� ���
		_cursor->setVisible(true);
		_selected = true;
		return true;
	}


	//�ٸ� ���� ������ ���
	if (_selected)
	{
		Point pos = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());
		this->stopAllActions();
		this->runAction(MoveTo::create(1.f, touch->getLocation()));
	}

}
