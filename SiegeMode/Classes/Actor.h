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
	void			setSpriteFromFileName(const char* fileName);
	CREATE_FUNC(Actor);
	void			refreshStroke();
private:

	std::array<Sprite*, 8>	_strokeArray;
	const float				_strokeSize = 1.f;
};