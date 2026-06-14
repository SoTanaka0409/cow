#pragma once

class Scene;
class ObjectManager;
class CowManager;
class AnimalManager;
class GameManager;
class Fever;
class Player3D;

class ServiceLocator
{
public:
	static Scene* GetCurrentScene();
	static ObjectManager* GetObjectManager();
	static Player3D* GetPlayer();
	static CowManager* GetCowManager();
	static AnimalManager* GetAnimalManager();
	static GameManager* GetGameManager();
	static Fever* GetFever();
};
