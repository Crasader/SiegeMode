#pragma once
#include "UnitState.h"

// Ÿ�ٿ��� �����Ÿ��� ���� ������ �����ϴ� ����
class UnitState_Approach : public UnitState
{
public:
	CREATE_FUNC(UnitState_Approach);
	bool init() override { return true; }
	void startState(Unit* unit) override;
	void runState(Unit* unit, float delta) override;
	void endState(Unit* unit) override;
};