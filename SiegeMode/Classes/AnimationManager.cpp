#include "pch.h"
#include "AnimationManager.h"

AnimationManager::AnimationManager()
{

}

AnimationManager* AnimationManager::_instance = nullptr;

Animation* AnimationManager::getAnimation(std::string actorName, std::string animName)
{
	auto rtnAnim = _animationMap.at(actorName+animName);
	if (rtnAnim != nullptr)
		return _animationMap.at(actorName+animName);
	//���� �������� ����
	return nullptr;
}

void AnimationManager::addAnimation(std::string actorName, std::string animName, float interval, std::initializer_list<std::string> frameName)
{
	if (_animationMap.at(actorName + animName) != nullptr)
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		//Beep(1000, 100);
#endif
		return;
	}

	auto anim = Animation::create();
	anim->setDelayPerUnit(interval);
	for (auto iter : frameName)
	{
		anim->addSpriteFrameWithFile(iter);
	}
	for (auto i : anim->getFrames())
	{
		i->getSpriteFrame()->getTexture()->setAliasTexParameters();
	}
	_animationMap.insert(actorName+animName,anim);
}

AnimationManager* AnimationManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new AnimationManager();
	return _instance;
}