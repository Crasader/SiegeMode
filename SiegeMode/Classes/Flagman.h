#pragma once
#include "Unit.h"

// �¸� ������ ���� �ӽ÷� ������ ����
// ������ ������ ������
class Flagman :
	public Unit
{
public:
	bool	init(DEF::PLAYER_KIND playerKind) override;
	void	kill() override;
	CREATE_FUNC_UNIT(Flagman);
};