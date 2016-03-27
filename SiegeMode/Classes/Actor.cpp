#include "pch.h"
#include "Actor.h"

bool Actor::init()
{
	/*
	 * �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	 */
	if (!this->Node::init())
		return false;

	this->setAnchorPoint(Vec2(0.5f, 0.5f));

	return true;
}

bool Actor::touchCallback(Touch* touch, Event* event)
{
	auto target = event->getCurrentTarget();
	Point pos = target->convertToNodeSpace(touch->getLocation());
	Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
	if (!rect.containsPoint(pos))
		return false;
	return true;
}