#include "pch.h"
#include "Unit.h"

bool Unit::init()
{
	/*
	* �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	*/
	if (!this->Actor::init())
		return false;
	this->setScale(2.f);

	return true;
}

bool Unit::touchCallback(Touch* touch, Event* event)
{
	this->refreshStroke();

	/*if (this->Actor::touchCallback(touch, event))
	{
		if (_selected)
		{
			_cursorUp->setVisible(false);
			_cursorDown->setVisible(false);
			_selected = false;
			return true;
		}
		//ó�� �����ϴ� ���
		_cursorUp->setVisible(true);
		_cursorDown->setVisible(true);
		_selected = true;
		return true;
	}


	//�ٸ� ���� ������ ���
	if (_selected)
	{
		Point pos = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());
		this->stopAllActions();
		this->runAction(MoveTo::create(1.f, touch->getLocation()));
	}*/
	return true;

}

cocos2d::Action* Unit::getActionItem(const std::string& actionName)
{
	return this->_actionList[actionName];
}
