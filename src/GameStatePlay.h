

#ifndef _GAME_STATE_PLAY_H_
#define _GAME_STATE_PLAY_H_

#include "GameState.h"
#include "GameApp.h"
#include <time.h>
#include <vector>
#include "Person.h"
#include "AI.h"
#include "TileMap.h"
#include "ParticleEngine.h"
#include "SfxManager.h"
#include "Bullet.h"
#include "GunObject.h"
#include "BuyMenu.h"
#include "Camera.h"
#include "Hud.h"
#include "Game.h"
#include "Collision.h"
#include "AStar.h"

#ifdef WIN32
#else
#include "mp3player.h"
#endif

class GameStatePlay:	public Game,
						public JGuiListener
						
{

private:

	AStar* mAStar;
	int iMoneyConf;
	/*float mRoundTimer;
	float mRoundEndTimer;
	int mRoundState;
	float mTimer;
	float mSwitchTimer;
	int mNumRemainingCTs;
	int mNumRemainingTs;
	unsigned int mSpecIndex;
	int mSpecState;
	bool mSpecDead;
	float mSpecX;
	float mSpecY;*/

	//JTexture* mGunTextures;

	int mNumGuns;

	//Person* mSpec;

	//vector<Bullet>::iterator mBulletIterator;

	JMusic* mMusic;
	
	//JGuiController* mGuiController;

	//bool cross;
	//int mWinner;
	bool mLRState;

public:
	GameStatePlay(GameApp* parent);
	~GameStatePlay();
	
	int MaxMoney;
	int MusicType;
	int mMaxBots;
	bool mCustomNames;
	bool mAllowRegeneration;
	

	void Create();
	void Destroy();
	void Start();
	void End();
	void Update(float dt);
	void Render();

	void CheckInput(float dt);
	void CheckCollisions();
	void NewGame();
	void ResetRound();
	//void ResetPlayer();
	void Explode(Grenade* grenade);
	void StopInput();

	void ButtonPressed(int controllerId, int controlId);
	
};


#endif

