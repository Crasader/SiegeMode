#include "pch.h"
#include "Unit.h"
#include "UnitManager.h"
#include "AnimationManager.h"
#include "json/json.h"

// �ʱ�ȭ �Լ�. CREATE ȣ��� �ڵ� ȣ��.
bool Unit::init(DEF::PLAYER_KIND ownerPlayer)
{
	Sprite::init();

	// ���� ���� �ҷ�����
	readSpecFromData();

#if _DEBUG_LABEL
	//����׿� �� ����
	_debugLabel = Label::createWithSystemFont("", "", 15);
	_debugLabel->setTextColor(Color4B(255, 0, 0, 255));
	_debugLabel->setScale(1.f / 2.f);
	addChild(_debugLabel);
	// ���ó�
#endif
	// ü�¹� ����
	_healthBar = Sprite::create("SpriteSource/UI/healthbar.png");
	_healthBar->setOpacity(0.7 * 255);
	_healthBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	addChild(_healthBar);
	// ���ó�

	_isDead = false;

	// ���������� �ѹ� �������ش�. ��������Ʈ ������ ���⶧��.
	setFlippedX(ownerPlayer == DEF::PLAYER_RED);

	_ownerPlayer = ownerPlayer;
	
	setScale(2.f);

	return true;
}

// �þ߿� ���� �� ������ �ϳ��� Ÿ�����Ѵ�.
// ������ Ÿ���� ������ nullptr ��ȯ
// UnitŬ������ �⺻������ ���� ����� ���� ��ȯ
Unit* Unit::scanTarget()
{
	auto candidates = getEnemyUnitsUnderSight();
	
	// �þ߿� ���� �� ������ ����
	if (candidates.size() == 0)
		return nullptr;
	

	Unit* currentTarget = nullptr;
	auto minDist = FLT_MAX;
	

	auto sign = _ownerPlayer == DEF::PLAYER_RED ? 1 : -1;
	for (auto i : candidates)
	{
		// ������ �ڿ� �ִ� ���� �߶󳽴�.
		if(sign * (i->getPositionX() - getPositionX()) < 0)
			continue;

		auto tempDist = getDistanceForRange(i->getPosition());
		// ���� ����� ���� ã�´�.
		if (minDist > tempDist)
		{
			minDist = tempDist;
			currentTarget = i;
		}
	}
	return currentTarget;
}

// �ܼ��� range���� ���������� ���� �������� ���ְ��� �Ÿ��� �˻��ϸ� �������� �ʹ� �д�.
// x�� ����ġ�� �ּ� �������� ���� Ÿ���÷� �Ÿ��� ���Ѵ�.
float Unit::getDistanceForRange(Vec2 range)
{
	auto convertedVec = getPosition() - range;
	convertedVec.y = convertedVec.y * 2.5;// y�Ÿ��� �÷������� ������ �������� �Ѵ�

	return convertedVec.getLength();
}

// �þ߿� ����ִ� �� ������ ��� ã�� ��ȯ
Vector<Unit*> Unit::getEnemyUnitsUnderSight()
{
	auto compareFunc = [](Unit* me, Unit* other)
	{
		if (me->getOwnerPlayer() == other->getOwnerPlayer())
			return false;
		if (me->getDistanceForRange(other->getPosition()) <= me->getSightRange())
			return true;
		return false;
	};

	auto returnVec = UnitManager::getInstance()->findUnitByCondition(
		this, compareFunc);
	return returnVec;
}

// ������ ���δ�.
// ��ü�� ��� �ı����� ������ �� ����
void Unit::kill()
{
	if (!_isDead)
	{
		unscheduleUpdate();
		removeFromParentAndCleanup(true);
		_isDead = true;
	}
}

// destination���� �����δ�.
// �Ļ�Ŭ�������� �������ؼ� anchorPoint�� �����־�� ��
void Unit::moveTo(Vec2 destination)
{
	stopMove();
	auto distance = destination.getDistance(getPosition());
	auto moveAction = MoveTo::create(1.f * distance / _moveSpeed, destination);
	moveAction->setTag(ACTION_MOVE);
	runAction(moveAction);
}

// directionVec �������� ������ �ð���ŭ �����δ�.
// duration�� 0�� ������ ���ѹݺ�
// directionVec�� ����ȭ�Ǿ����� �ʾƵ� ��
// �Ļ�Ŭ�������� �������ؼ� anchorPoint�� �����־�� ��
void Unit::moveBy(Vec2 directionVec, float duration)
{
	// ���߱� ����
	if (duration != 0.f)
		runAction(
			Sequence::create(
				DelayTime::create(duration + 0.01f), //0.01f ����� �ȵ�
				CallFunc::create(CC_CALLBACK_0(Unit::stopMove, this)), nullptr));

	// ���⺤�͸� ����ȭ
	directionVec = directionVec.getNormalized();


	if (duration == 0.f)// ������ �ȱ�
	{
		auto moveAction = RepeatForever::create(MoveBy::create(1.f, directionVec * _moveSpeed));
		moveAction->setTag(ACTION_MOVE);
		runAction(moveAction);
	}
	else// ������ �ð����� �ȱ�
	{
		auto moveAction = MoveBy::create(1.f * duration, directionVec * _moveSpeed * duration);
		moveAction->setTag(ACTION_MOVE);
		runAction(moveAction);
	}

}

// moveTo, moveBy ���� �Լ�
// �׸� �ȱ�
void Unit::stopMove()
{
	stopAllActionsByTag(ACTION_MOVE);
}

// �������� �ִϸ��̼��� �ߴ�
void Unit::stopAnimation()
{
	stopAllActionsByTag(ACTION_ANIMATE);
}

// �ִϸ��̼� ����
void Unit::startAnimate(std::string animName, bool isRepeatForever)
{
	// �������̴� �ִϸ��̼� �ߴ�
	stopAnimation();
	
	auto animation = AnimationManager::getInstance()->getAnimation(_unitName, animName + (_ownerPlayer == DEF::PLAYER_RED ? "_red" : "_blue"));
	Action* animate;
	if (isRepeatForever)
		animate = RepeatForever::create(Animate::create(animation));
	else
		animate = Animate::create(animation);
	
	animate->setTag(ACTION_ANIMATE);
	runAction(animate);
}

// ������ �¾��� ���� ����
// ������ ������ ������ �� �Լ��� ȣ������ ���� scheduleBeHit�� ȣ���� ��
void Unit::beHit(unsigned attackPower)
{
	_health -= attackPower;
	
	//ü�¹� ����
	_healthBar->setScaleX((float)_health / (float)_maxHealth);
	
	// �¾��� �� ������ ��½ �ϴ� ȿ��
	runAction(Sequence::create(TintTo::create(0.03f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE), nullptr));
	
	//�´� �Ҹ�
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/hit.wav");
	
	if (_health <= 0)
		kill();
}

// ������ ������ ���� �� �� �Լ��� ȣ��
void Unit::scheduleBeHit(unsigned attackPower, float delay)
{
	runAction(Sequence::create(
		DelayTime::create(delay), CallFuncN::create(
			CC_CALLBACK_0(Unit::beHit, this, attackPower)
		), nullptr));
}

// ���� ������ ���� x���� ���������� �� �����Ƿ� ��Ŀ����Ʈ�� �ٸ��� ����־����
void Unit::setAnchorPoint(const Vec2& anchor)
{
	if (_ownerPlayer == DEF::PLAYER_BLUE)
		Sprite::setAnchorPoint(anchor);
	else
		Sprite::setAnchorPoint(Vec2(1.f - anchor.x,anchor.y));
}


// Ÿ���� �� ���ݹ����� ������� �˻�
bool Unit::isRightTarget(Unit* target)
{
	if (getDistanceForRange(getAttackTarget()->getPosition()) <= getAttackRange())
		return true;
	return false;
}

// ���� ������ �ҷ��´�.
// ���ϴ� �� �Լ��� �⺻���� �ҷ����� ����
// attackDelay, accuracy, attackPower, attackRange, attackSpeed, maxHealth, moveSpeed, sightRange
// �� ��� �̿��� �Ļ� Ŭ�������� �߰������� ������ ������ �Ļ�Ŭ�������� �� �Լ��� �������Ͽ� �˾Ƽ� �ҷ��´�.
void Unit::readSpecFromData()
{
	auto specData = UnitManager::getInstance()->getSpecData();
	auto currentUnitData = specData.get(_unitName,"failed");
	auto getData = [&currentUnitData](auto key, auto defaultVal) {return currentUnitData.get(key, defaultVal); };
	setAttackDelay(getData("attackDelay", 0.f).asFloat());
	setAttackAccuracy(getData("accuracy", 0.f).asFloat());
	setAttackPower(getData("attackPower", 0).asInt());
	setAttackRange(getData("attackRange", 0.f).asFloat());
	setAttackSpeed(getData("attackSpeed", 0.f).asFloat());
	setMaxHealth(getData("maxHealth", 0).asInt());
	setHealth(getMaxHealth());
	setMoveSpeed(getData("moveSpeed", 0.f).asFloat());
	setSightRange(getData("sightRange", 0.f).asFloat());
}

// �� ������ ȣ��
void Unit::update(float delta)
{
	// ���� �ִϸ��̼� ������ ���� �����ϴ� �ͺ��� �������� ����ִ� ���� �� ���ٰ� �Ǵ�.
	_healthBar->setPosition(_contentSize.width * _anchorPoint.x,0.f);

	// z�� ����
	setLocalZOrder(-_position.y);
	
	// �����ڵ�
	_state->runState(this, delta);
}

// UnitManager���� tag������ ���� attackTarget�� ���´�.
// target�� ���� �����͸� ���� �ٷ��� �ʰ� tag�� ����ִ�.
// if failed, it will return nullptr
Unit* Unit::getAttackTarget()
{
	return UnitManager::getInstance()->getUnitByTag(_tagAttackTarget);
}
void Unit::setAttackTargetByTag(int targetTag)
{
	if (UnitManager::getInstance()->getUnitByTag(targetTag))
		_tagAttackTarget = targetTag;
}

// Ÿ������ ����� �� �� �����Ѵ�.
void Unit::attackOnce()
{
	startAnimate("attack", false);
	if (RandomHelper::random_real(0.f, 1.f) <= _attackAccuracy)
		getAttackTarget()->scheduleBeHit(_attackPower, _attackDelay);
}