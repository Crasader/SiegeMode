#include "pch.h"
#include "UnitTest.h"

bool UnitTest::init()
{
	/*
	* �θ� Ŭ������ init()�� ���ǻ� ȣ�����ش�.
	*/
	if (!this->Unit::init())
		return false;

	return true;
}