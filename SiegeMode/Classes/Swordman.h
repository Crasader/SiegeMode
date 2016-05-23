#pragma once
#include "Unit.h"

// Unit�� �Ļ� Ŭ������ ���� ������ ����.
class Swordman :
	public Unit
{
public:
	bool	init(DEF::PLAYER_KIND playerKind) override;
	void	moveTo(Vec2 destination) override;
	void	moveBy(Vec2 directionVec, float duration) override;
	void	attackOnce() override;
	CREATE_FUNC_UNIT(Swordman);
};