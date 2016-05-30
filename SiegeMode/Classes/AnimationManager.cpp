#include "pch.h"
#include "AnimationManager.h"
#include "json/json.h"

AnimationManager* AnimationManager::_instance = nullptr;

AnimationManager::AnimationManager()
{
	// json �Ľ�
	auto tmpData = FileUtils::getInstance()->getStringFromFile("Data/animations.json");
	Json::Value animData;
	Json::Reader reader;
	reader.parse(tmpData, animData);
	auto units = animData.getMemberNames();
	for (auto currentUnitName : units)
	{
		auto anims = animData.get(currentUnitName, "failed");
		for (auto currentAnim : anims)
		{
			auto animName = currentAnim.get("animName", "failed").asString();
			auto interval = currentAnim.get("interval", 0.f).asFloat();
			auto width = currentAnim.get("width", 0).asInt();
			auto height = currentAnim.get("height", 0).asInt();
			auto nFrame = currentAnim.get("nFrame", 0).asInt();
			auto fileName = "SpriteSource/" + currentUnitName + "/" + currentUnitName + "_" + animName + ".png";
			addAnimation(currentUnitName, animName, interval, fileName, width, height, nFrame);
		}
	}
}

// �ִϸ��̼��� ã�Ƽ� ��ȯ
Animation* AnimationManager::getAnimation(std::string actorName, std::string animName)
{
	auto rtnAnim = _animationMap.at(actorName + animName);
	if (rtnAnim != nullptr)
		return _animationMap.at(actorName + animName);

	//���� �������� ����
	return nullptr;
}


// �ִϸ��̼��� �߰�
// actorName		: ���� �̸�
// animName			: �ִϸ��̼� �̸�
// interval			: ������ �� ������
// fileName			: ��������Ʈ �����̸�
// width, height	: ��������Ʈ �� �����Ӵ� ����, ����
// nFrame			: ��������Ʈ ������ �� 
void AnimationManager::addAnimation(std::string actorName, std::string animName, float interval, std::string fileName, unsigned width, unsigned height, unsigned nFrame)
{
	// �̹� �����ϴ� �����̸� ���� �߰� ����
	if (_animationMap.at(actorName + animName) != nullptr)
		return;


	Vector<SpriteFrame*> animFrames(nFrame);
	
	for (auto i = 1u; i <= nFrame; ++i)// �������� �ϳ��� �߶� ���Ϳ� �ִ´�
	{
		auto frame = SpriteFrame::create(fileName, Rect((i - 1)*width, 0, width, height));
		animFrames.pushBack(frame);
	}

	auto anim = Animation::createWithSpriteFrames(animFrames, interval);
	for (auto i : anim->getFrames())//Alias ����(��Ʈ �̹����̹Ƿ�)
		i->getSpriteFrame()->getTexture()->setAliasTexParameters();

	anim->setDelayPerUnit(interval);
	_animationMap.insert(actorName + animName, anim);
}

// �̱���
AnimationManager* AnimationManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new AnimationManager();
	return _instance;
}