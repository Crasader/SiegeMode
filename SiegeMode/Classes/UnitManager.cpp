#include "pch.h"
#include "UnitManager.h"
#include "Swordman.h"
#include "Bowman.h"
#include "Flagman.h"
#include "Balistar.h"

UnitManager* UnitManager::_instance = nullptr;

// �̱���
UnitManager* UnitManager::getInstance()
{
	if (!_instance)
		_instance = new UnitManager();
	return _instance;
}

// ���� �̸��� �޾Ƽ� ��ȯ�Ѵ�.
// ���ø����� �������� �ʰ� ���ڿ��� �޴� ������ ȣ���ϴ� �ʿ��� ���� ������ ���� include�� ���ϱ� ����
void UnitManager::summonUnit(std::string unitName, Vec2 position, DEF::PLAYER_KIND ownerPlayer)
{
	// �±� ������� ���ָ���� �����Ѵ�
	static int tagid = 1;
	Unit* newUnit = nullptr;
	
	if (unitName == "swordman")			newUnit = Swordman::create	(ownerPlayer);
	else if (unitName == "bowman")		newUnit = Bowman::	create	(ownerPlayer);
	else if (unitName == "flagman")		newUnit = Flagman::	create	(ownerPlayer);
	else if (unitName == "balistar")	newUnit = Balistar::create	(ownerPlayer);

	newUnit->setTag(++tagid);
	newUnit->setPosition(position);
	_unitList->addChild(newUnit);
}

Unit* UnitManager::getUnitByTag(int tag)
{
	return static_cast<Unit*>(_unitList->getChildByTag(tag));
}

// ���ǽ��� �޾Ƽ� ���ǿ� �´� ������ ��ȯ��
Vector<Unit*> UnitManager::findUnitByCondition(Unit* caller, std::function<bool(Unit* caller, Unit* other)> compare)
{
	auto children = _unitList->getChildren();
	Vector<Unit*> returnVec;
	for (auto i : children)
	{
		auto currentUnit = static_cast<Unit*>(i);
		//���� ����
		if (compare(caller, currentUnit))
			returnVec.pushBack(currentUnit);
	}
	return returnVec;
}

void UnitManager::deleteInstance()
{
	_unitList->release();
	delete _instance;
	_instance = nullptr;
}

UnitManager::UnitManager()
{
	_unitList = Node::create();
	_unitList->retain();

	// json �����͸� �̸� �Ľ��س��´�.
	auto tmpData = FileUtils::getInstance()->getStringFromFile("Data/unit_spec.json");
	Json::Reader reader;
	reader.parse(tmpData, _specData);
}