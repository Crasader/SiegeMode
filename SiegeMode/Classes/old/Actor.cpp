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


	for (auto& i : _strokeArray)
	{
		i = Sprite::create();
		this->addChild(i, this->getLocalZOrder() - 1);
	}
	return true;
}

bool Actor::screenTouchCallback(Touch* touch, Event* event)
{
	auto target = event->getCurrentTarget();
	Point pos = target->convertToNodeSpace(touch->getLocation());
	Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
	if (!rect.containsPoint(pos))
		return false;

	return true;
}

void Actor::setSpriteFromFileName(const char* fileName)
{
	this->initWithSpriteFrameName(fileName);
}

void Actor::refreshStroke()
{
	Image* strokeImg = new Image();

	//��Ʈ��ũ ĥ�� ��������Ʈ���� �����̸��� �̾ƿͼ� �̹����� �д´�.
	strokeImg->initWithImageFile(settings::PATHNAME_SPRITE_FOLDER +  "/" + this->_fileName);

	auto x = 3;
	if (strokeImg->hasAlpha())
		x = 4;
	auto pixelData = new unsigned char[strokeImg->getDataLen()*x];
	pixelData = strokeImg->getData();

	//���İ� ���ΰ� �� �о������.
	for (int j = 0; j < strokeImg->getWidth(); ++j)
	{
		for (int i = 0; i < strokeImg->getHeight(); ++i)
		{
			auto pixel = pixelData + (i + j*strokeImg->getWidth()) * x;
			auto r = pixel;
			auto g = (pixel + 1);
			auto b = (pixel + 2);
			auto a = (pixel + 3);
			if (*a > 0)
			{
				*r = 0;
				*g = 255;
				*b = 0;
			}
		}
	}


	//������ �̹����� 8�� ���.
	auto newTexture = new Texture2D();
	newTexture->initWithImage(strokeImg);
	newTexture->setAliasTexParameters();
	for (auto i : _strokeArray)
	{
		i->initWithTexture(newTexture);
		i->setAnchorPoint(Vec2(0, 0));
	}
	auto strokeSizeInPixel = _strokeSize / this->getScale();
	_strokeArray[0]->setPosition(Vec2(+strokeSizeInPixel, +strokeSizeInPixel));
	_strokeArray[1]->setPosition(Vec2(+strokeSizeInPixel, 0));
	_strokeArray[2]->setPosition(Vec2(+strokeSizeInPixel, -strokeSizeInPixel));
	_strokeArray[3]->setPosition(Vec2(-strokeSizeInPixel, +strokeSizeInPixel));
	_strokeArray[4]->setPosition(Vec2(-strokeSizeInPixel, 0));
	_strokeArray[5]->setPosition(Vec2(-strokeSizeInPixel, -strokeSizeInPixel));
	_strokeArray[6]->setPosition(Vec2(0, +strokeSizeInPixel));
	_strokeArray[7]->setPosition(Vec2(0, -strokeSizeInPixel));
}

void Actor::touchThisCallback()
{

}

