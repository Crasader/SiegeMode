#include "pch.h"
#include "UnitState_WalkAndSeek.h"
#include "AnimationManager.h"
#include "UnitState_Approach.h"
#include "Unit.h"


void UnitState_WalkAndSeek::startState(Unit* unit)
{
	// ���� ������ ������ ���� �޶���
	auto moveDirection = unit->getOwnerPlayer() == DEF::PLAYER_BLUE ? -1 : 1;

	unit->startAnimate("walk", true);
	unit->moveBy(Vec2(moveDirection, 0), 0.f);
#if _DEBUG_LABEL
	unit->getDebugLabel()->setString("walking!");
#endif
}

void UnitState_WalkAndSeek::runState(Unit* unit, float delta)
{
	// ȭ�� ����� ����
	if (unit->getPositionX() < 0 || unit->getPositionX() > DEF::SCREEN_WIDTH)
	{
		unit->kill();
		return;
	}

	// ������ Ÿ���� �ֳ� �˻�
	auto properTarget = unit->scanTarget();
	if (properTarget)//������
	{
		// Ÿ������ ����
		unit->setAttackTargetByTag(properTarget->getTag());
		// Ÿ���� ���� ����
		unit->changeState<UnitState_Approach>();
	}
}

void UnitState_WalkAndSeek::endState(Unit* unit)
{
	unit->stopAnimation();
	unit->stopMove();
}