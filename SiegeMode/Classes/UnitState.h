#pragma once

class Unit;

// Unit�� ��Ÿ�� ������ ������ State
class UnitState : public Component
{
public:
	// State ���۽� �ڵ�
	virtual void startState(Unit* unit) = 0;
	// �� ������ ȣ��� �ڵ�
	virtual void runState(Unit* unit, float delta) = 0;
	// State ������ �ڵ�
	virtual void endState(Unit* unit) = 0;
};