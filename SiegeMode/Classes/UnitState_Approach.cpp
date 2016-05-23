#include "pch.h"
#include "UnitState_Approach.h"
#include "AnimationManager.h"
#include "Unit.h"
#include "UnitState_Attack.h"
#include "UnitState_WalkAndSeek.h"

void UnitState_Approach::startState(Unit* unit)
{
	unit->startAnimate("walk", true);
	// ����׿�
#if _DEBUG_LABEL
	unit->getDebugLabel()->setString("Approaching!!");
#endif
}

void UnitState_Approach::runState(Unit* unit, float delta)
{
	auto a = unit->getOwnerPlayer() == DEF::PLAYER_BLUE ? 1 : -1;
	if (unit->getAttackTarget() == nullptr// Ÿ���� �׾��ų� 
		|| a*unit->getAttackTarget()->getPositionX() > a*unit->getPositionX()// Ÿ���� ������ �ڿ� ������
		)
	{
		// �ٽ� Ž��
		unit->changeState<UnitState_WalkAndSeek>();
		return;
	}

	// Ÿ�������� �������� ���ݾ� �ɾ��
	auto normalVec = (unit->getAttackTarget()->getPosition() - unit->getPosition()).getNormalized();
	normalVec *= delta * unit->getMoveSpeed();
	unit->setPosition(unit->getPosition() + normalVec);
	if (unit->getPosition().distance(unit->getAttackTarget()->getPosition()) <= unit->getAttackRange())
		unit->changeState<UnitState_Attack>();
}

void UnitState_Approach::endState(Unit* unit)
{
	unit->stopAnimation();
	unit->stopMove();
}