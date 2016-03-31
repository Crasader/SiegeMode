#pragma once
#include <array>

class Actor : public Sprite
{
public:
	/*
	 * Node���� ����� init() �޼ҵ带 �������Ѵ�.
	 * CREATE_FUNC(PAWN)�� ���� �̷�.
	 * create()�� ������ �Ŀ� ������ �������� ȣ��Ǵ� �޼ҵ��̴�.
	 */
	virtual bool	init();

	virtual bool	touchCallback(Touch* touch, Event* event);

	CREATE_FUNC(Actor);
	void refreshStroke();
private:

	std::array<Sprite*, 4> _strokeArray;
	Sprite* asdf;
};