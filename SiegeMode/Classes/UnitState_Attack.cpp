#include "pch.h"
#include "UnitState_Attack.h"
#include "AnimationManager.h"
#include "UnitState_Approach.h"
#include "Unit.h"
#include "UnitState_WalkAndSeek.h"

void UnitState_Attack::startState(Unit* unit)
{
#if _DEBUG_LABEL
	unit->getDebugLabel()->setString("Attacking!!");
#endif
	if (unit->getAttackTarget() == nullptr)
	{
		unit->changeState<UnitState_WalkAndSeek>();
		return;
	}
	// �ϴ� �Ѵ� ������
	unit->attackOnce();
}

void UnitState_Attack::runState(Unit* unit, float delta)
{
	_elapsedTimeFromLastAttack += delta;

	// �ð��� ������ ������
	if (_elapsedTimeFromLastAttack >= unit->getAttackSpeed())
	{
		//Ÿ���� �׾��ų�, ���� �����ưų�, ������ ������� �ٽ� �ȴ´�.

		auto a = unit->getOwnerPlayer() == DEF::PLAYER_RED ? 1 : -1;

		if (unit->getAttackTarget() != nullptr)
		{// Ÿ���� ��ȿ�ؾ���
			if (a*(unit->getAttackTarget()->getPositionX() - unit->getPositionX()) >= 0)
			{// Ÿ���� �� �ڷ� ���� �ȵ�
				if(unit->isRightTarget(unit->getAttackTarget()))
				{// Ÿ���� �� ���ݹ��� �ȿ� ����־�ߵ�
					unit->attackOnce();
					_elapsedTimeFromLastAttack = 0.0;
					return;
				}
			}
		}
		// �� ���� �ϳ��� ���� ���ϸ� ���� ã�ƾߵ�
		unit->changeState<UnitState_WalkAndSeek>();
		return;
	}

}

void UnitState_Attack::endState(Unit* unit)
{
}