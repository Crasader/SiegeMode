#include "pch.h"
#include "UnitState_Approach.h"
#include "AnimationManager.h"
#include "Unit.h"
#include "UnitState_Attack.h"

void UnitState_Approach::startState(Unit* unit)
{
	unit->startAnimate("walk", true);
	unit->getDebugLabel()->setString("Approaching!!");
}

void UnitState_Approach::runState(Unit* unit, float delta)
{
	auto normalVec = (unit->getAttackTarget()->getPosition() - unit->getPosition()).getNormalized();
	normalVec *= delta * unit->getMoveSpeed();
	unit->setPosition(unit->getPosition() + normalVec);

	if (unit->getPosition().distance(unit->getAttackTarget()->getPosition()) <= unit->getAttackRange())
		unit->changeState<UnitState_Attack>();

}

void UnitState_Approach::endState(Unit* unit)
{
	unit->stop();
}