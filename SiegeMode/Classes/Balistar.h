#pragma once
#include "Unit.h"

// Bowman�� �Ȱ��� �ۼ��Ǿ���.
// ���� �ڵ� ���� ��� �����.
class Balistar :
	public Unit
{
public:
	bool			init(DEF::PLAYER_KIND playerKind) override;
	void			attackOnce() override;
	void			moveBy(Vec2 directionVec, float duration) override;
	void			moveTo(Vec2 destination) override;
	void			shootArrow(Vec2 targetPos);
	virtual Unit *	scanTarget();
	virtual bool	isRightTarget(Unit* target);

	CREATE_FUNC_UNIT(Balistar);
	CC_SYNTHESIZE(float, _arrowSpeed, ArrowSpeed);
	CC_SYNTHESIZE(float, _attackRangeMin, AttackRangeMin);
protected:
	void	readSpecFromData() override;
private:
	float _arrowTime;

};