#pragma once

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

};
