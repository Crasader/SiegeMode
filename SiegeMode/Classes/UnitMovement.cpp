#include "pch.h"
#include "UnitMovement.h"

bool UnitMovement::init()
{
	/*
	* �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	*/
	if (!this->ActorComponent::init())
		return false;
	return true;
}