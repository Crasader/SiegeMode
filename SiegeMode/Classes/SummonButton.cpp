#include "pch.h"
#include "SummonButton.h"
#include "UnitManager.h"

std::string SummonButton::g_selectedUnitName = "";
Rect SummonButton::g_summonableZone = Rect(DEF::FIGHTING_ZONE.origin,Size(DEF::FIGHTING_ZONE.size.width/3.f,DEF::FIGHTING_ZONE.size.height));

SummonButton* SummonButton::create(Vec2 pos, std::string unitName)
{
	g_selectedUnitName = "";

	auto newInst = SummonButton::create();
	newInst->setPosition(pos);
	newInst->setGlobalZOrder(DEF::ZORDER_UI);
	newInst->setAnchorPoint(Vec2(0.f, 0.f));
	newInst->setUnitName(unitName);
	newInst->scheduleUpdate();

	// �ڽ� ��������Ʈ�鿡 ���� �ϰ��۾�
	auto tempFactory = [=](std::string fileName, int zorder) {
		constexpr auto zOrigin = 0;
		auto tempSpr = Sprite::create(fileName);
		tempSpr->setLocalZOrder(zOrigin + zorder);
		tempSpr->getTexture()->setAliasTexParameters();
		tempSpr->setGlobalZOrder(DEF::ZORDER_UI);
		newInst->addChild(tempSpr);
		return tempSpr;
	};
	// newFrame		: �׵θ� ����
	// newCover		: ��Ÿ�� ǥ���� ����
	// newUnitImage : �ش� ���� �̹���
	// ���ϴ� ������ �ٲ��� �ʴ� �̻� �ٲ� ���� ���� ���� ���ͷ����̹Ƿ� �׳� ���
	auto newFrame = tempFactory("SpriteSource/UI/unit_frame.png", 0);
	auto newCover = tempFactory("SpriteSource/UI/framecover.png", 2);
	auto newUnitImage = tempFactory("SpriteSource/UI/" + unitName + ".png", 1);
	newInst->setFrameCover(newCover);
	newInst->setFrame(newFrame);
	newInst->setUnitImage(newUnitImage);
	newCover->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	newCover->setPositionY(newFrame->getContentSize().height / -2.f);
	newCover->setOpacity(255 * 0.7f);
	newCover->setScaleY(0.f);

	auto path = "SpriteSource/UI/" + unitName + "_preview.png";
	newInst->setUnitPreview(Sprite::create(path));
	newInst->getUnitPreview()->setVisible(false);
	newInst->getUnitPreview()->getTexture()->setAliasTexParameters();
	newInst->addChild(newInst->getUnitPreview());
	newInst->getUnitPreview()->setFlippedX(true);
	newInst->getUnitPreview()->setGlobalZOrder(DEF::ZORDER_UI);


	// ��ġ�ݹ� ���
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SummonButton::onSprTouchBegan, newInst);
	listener->onTouchEnded = CC_CALLBACK_2(SummonButton::onSprTouchEnd, newInst);
	listener->onTouchMoved = CC_CALLBACK_2(SummonButton::onSprTouchMoved, newInst);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, newInst);


	return newInst;
}

// �� ������ ȣ��
void SummonButton::update(float delta)
{
	// ��Ÿ���� ��� ����ϰ� �ٲ� �� ������
	static float coolTime;
	if (_unitName == "swordman")
		coolTime = 6.f;
	else if (_unitName == "bowman")
		coolTime = 8.f;
	else if (_unitName == "balistar")
		coolTime = 30.f;

	// �ش� ��ư ���ÿ��� ǥ��
	if (g_selectedUnitName == _unitName)
		_frame->setColor(Color3B::GREEN);
	else
		_frame->setColor(Color3B::WHITE);

	// ��Ÿ���� ���� ���� ������ ���� �ٿ��ش�.
	if (_frameCover->getScaleY() > 0.f)
		_frameCover->setScaleY(_frameCover->getScaleY() - delta / coolTime);

	updateUnitPreviewColor();
}

// ��ġ�� �� ���� ��ȯ
void SummonButton::onSprTouchEnd(Touch* touch, Event* event)
{
	Point pos = touch->getLocation() + Vec2(0, 50);
	if (isSummonAble())
	{
		UnitManager::getInstance()->summonUnit(_unitName, pos, DEF::PLAYER_RED);
		// ��Ÿ�� ���� ����
		_frameCover->setScaleY(1.f);
	}
	_unitPreview->setVisible(false);
}

// �����̸� ����ٴ�
void SummonButton::onSprTouchMoved(Touch* touch, Event* event)
{
	if (g_selectedUnitName != _unitName)
		return;
	updateSummonPoint(touch);
	updateUnitPreviewColor();
	_unitPreview->setVisible(true);
	_unitPreview->setPosition(_summonPoint - _position + Vec2(0, 20));
}

void SummonButton::updateUnitPreviewColor()
{
	if (isSummonAble())// ��ȯ ���ɽ� �ʷϻ�
		_unitPreview->setColor(Color3B::GREEN);
	else if (g_summonableZone.containsPoint(_summonPoint))// ��Ÿ���̸� ��Ȳ��
		_unitPreview->setColor(Color3B::ORANGE);
	else// ��ȯ�Ұ������̸� ������
		_unitPreview->setColor(Color3B::RED);

}

// Ŭ�� ȿ��
void SummonButton::showOverlay()
{
	// ------- ���� Ƣ�� ȿ�� ------
	runAction(
		EaseElasticOut::create(
			Sequence::create(
				ScaleTo::create(0.3f, 1.05f),
				ScaleTo::create(0.4f, 1.00f),
				nullptr)));

	// ------ �������� ȿ�� ------
	auto overlay = Sprite::createWithTexture(_frame->getTexture());

	// ��� ������ ȿ���� �ֱ� ���� ������ �����Ѵ�.
	BlendFunc add;
	add.dst = GL_ONE;
	add.src = GL_SRC_ALPHA;
	overlay->setBlendFunc(add);

	overlay->runAction(
		Sequence::create(
			ScaleTo::create(0.4f, getScale() * 1.2f), RemoveSelf::create(true), nullptr));
	overlay->runAction(
		FadeOut::create(0.4f));
	overlay->setPosition(getPosition());

	getParent()->addChild(overlay);
}

// ��ȯ �������� �Ǻ�
bool SummonButton::isSummonAble()
{
	if (_frameCover->getScaleY() > 0.f)// ��Ÿ��
		return false;
	if (!g_summonableZone.containsPoint(_summonPoint))// ��ȯ��������������
		return false;
	if (g_selectedUnitName != _unitName)// ���� ���õ� ���� ������
		return false;
	return true;
}

// ��ȯ�ϰ���� ����Ʈ ����
void SummonButton::updateSummonPoint(Touch* touch)
{
	_summonPoint = touch->getLocation() + Vec2(0, 50);
}

// ��ġ ����
bool SummonButton::onSprTouchBegan(Touch* touch, Event* event)
{

	//��ư�� Ŭ���Ǿ����� Ȯ��
	Point pos = _frame->convertToNodeSpace(touch->getLocation());
	updateSummonPoint(touch);
	Rect currentFrameRect = Rect(0, 0, _frame->getContentSize().width, _frame->getContentSize().height);
	if (currentFrameRect.containsPoint(pos))
	{
		g_selectedUnitName = _unitName;
		showOverlay();
		return true;
	}
	if (!DEF::FIGHTING_ZONE.containsPoint(_summonPoint))
		return false;

	// ���� ���õȳ��� ���� ������ ȭ�鿡 �����ش�
	if (g_selectedUnitName == _unitName)
	{
		updateUnitPreviewColor();
		_unitPreview->setVisible(true);
		_unitPreview->setPosition(_summonPoint - _position + Vec2(0,20));
		return true;
	}
	return false;
}