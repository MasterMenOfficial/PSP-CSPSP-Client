#include "GameStateOptions.h"

GameStateOptions::GameStateOptions(GameApp* parent): GameState(parent)
{
	mState = GENERAL;
	// GENERAL
	// GAME
	// CONTROLS
	index = 0;
	main = true;
	gLastKey = 0;
	gKeyRepeatDelay = 0;
	mControlPage = 1;
}

GameStateOptions::~GameStateOptions() {}

void GameStateOptions::Create()
{
	movementstyle = ABSOLUTE1;
	music = OFF;
	friendlyfire = ON;
	menustyle = ANALOG;


	//initial check to use nickname
	#ifdef WIN32
	#else
	char* nameconfig = GetConfig("data/config.txt","name");
	if (nameconfig != NULL) {
		strncpy(name,nameconfig,15);
		delete nameconfig;
	}
	else {
		strcpy(name,"default");
	}
	name[15] = '\0';
	FILE* file = fopen("nataku92.txt","r");
	if (file == NULL) {
		if (stricmp(name,"nataku92") == 0 || strstr(name,"nataku92") != NULL) {
			strcpy(name,"default");
		}
	}
	else {
		fclose(file);
	}

	if (strcmp(name,"default") == 0) {
		sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, name, 15);
		name[15] = '\0';

		movementstyle = ABSOLUTE1;
		char* movement = GetConfig("data/config.txt","movement");
		if (movement != NULL) {
			if (strcmp(movement,"relative") == 0) {
				movementstyle = RELATIVE1;
			}
			else if (strcmp(movement,"absolute") == 0) {
				movementstyle = ABSOLUTE1;
			}
			delete movement;
		}

		music = OFF;
		char* musicconfig = GetConfig("data/config.txt","music");
		if (musicconfig != NULL) {
			if (strcmp(musicconfig,"on") == 0) {
				music = ON;
			}
			else if (strcmp(musicconfig,"off") == 0) {
				music = OFF;
			}
			delete musicconfig;
		}

		friendlyfire = ON;
		char* ff = GetConfig("data/config.txt","ff");
		if (ff != NULL) {
			if (strcmp(ff,"on") == 0) {
				friendlyfire = ON;
			}
			else if (strcmp(ff,"off") == 0) {
				friendlyfire = OFF;
			}
			delete ff;
		}

		menustyle = ANALOG;
		char* menu = GetConfig("data/config.txt","menu");
		if (menu != NULL) {
			if (strcmp(menu,"analog") == 0) {
				menustyle = ANALOG;
			}
			else if (strcmp(menu,"dirpad") == 0) {
				menustyle = DIRPAD;
			}
			delete menu;
		}
		
		// MusicSettings.txt
			MusicType = 1;
			char* MusType = GetConfig("data/MusicSettings.txt","MusicType");
			if (MusType != NULL) {
				if (strcmp(MusType,"1") == 0) {
					MusicType = 1;
				}
				if (strcmp(MusType,"2") == 0) {
					MusicType = 2;
				}
				else {
					MusicType = 1;
				}
				
				delete MusType;
			}
			
			//LoadMusic
			LoadMusic = 1;
			char* LoadMus = GetConfig("data/MusicSettings.txt","LoadMusic");
			if (LoadMus != NULL) {
				if (strcmp(LoadMus,"1") == 0) {
					LoadMusic = 1;
				}
				else if (strcmp(LoadMus,"2") == 0) {
					LoadMusic = 2;
				}
				else if (strcmp(LoadMus,"3") == 0) {
					LoadMusic = 3;
				}
				
				delete LoadMus;
			}
			

		Save();
	}
	#endif


	for (int i=0; i<NUMCONFIGS; i++) {
		mGuiControllers[i] = NULL;
		strcpy(mConfigs[i],"");
		strcpy(mConfigInfo[i],"");
	}
	float x = 160+10;

	mGuiControllers[0] = new JGuiController(0, this, JGUI_STYLE_LEFTRIGHT);
	mGuiControllers[0]->Add(new MenuItem(RELATIVE1, gFont, "Relative", x, 40, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[0]->Add(new MenuItem(ABSOLUTE1, gFont, "Absolute", x+100, 40, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[0]->SetActive(false);
	strcpy(mConfigs[0],"Movement Style");
	strcpy(mConfigInfo[0],"Change your player's movement style");

	mGuiControllers[1] = new JGuiController(1, this, JGUI_STYLE_LEFTRIGHT);
	mGuiControllers[1]->Add(new MenuItem(ON, gFont, "ON", x, 65, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[1]->Add(new MenuItem(OFF, gFont, "OFF", x+50, 65, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[1]->SetActive(false);
	strcpy(mConfigs[1],"Music");
	strcpy(mConfigInfo[1],"Enable or disable the background music in singleplayer");

	mGuiControllers[2] = new JGuiController(2, this, JGUI_STYLE_LEFTRIGHT);
	mGuiControllers[2]->Add(new MenuItem(ON, gFont, "ON", x, 90, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[2]->Add(new MenuItem(OFF, gFont, "OFF", x+50, 90, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[2]->SetActive(false);
	strcpy(mConfigs[2],"Friendly Fire");
	strcpy(mConfigInfo[2],"Enable or disable friendly fire in singleplayer");

	mGuiControllers[3] = new JGuiController(3, this, JGUI_STYLE_LEFTRIGHT);
	mGuiControllers[3]->Add(new MenuItem(ANALOG, gFont, "Analog", x, 115, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[3]->Add(new MenuItem(DIRPAD, gFont, "Dir pad", x+100, 115, TYPE_OPTION, JGETEXT_LEFT));
	mGuiControllers[3]->SetActive(false);
	strcpy(mConfigs[3],"Menu Style");
	strcpy(mConfigInfo[3],"Change the style of the in-game team select and buy menu");

	strcpy(mConfigs[4],"Name");
	strcpy(mConfigInfo[4],"Choose a name (15 characters max)");

	strcpy(mConfigs[NUMCONFIGS-1],"Save");
	strcpy(mConfigInfo[NUMCONFIGS-1],"Save any changes and return to the main menu");
	//strcpy(mConfigs[NUMCONFIGS-1],"cancel");
	//strcpy(mConfigInfo[NUMCONFIGS-1],"Ignore any changes and return to the main menu");

	mControlsTex1 = mRenderer->LoadTexture("gfx/controls1.png");
	mControlsQuad1 = new JQuad(mControlsTex1,0,0,480,272);
	mControlsTex2 = mRenderer->LoadTexture("gfx/controls2.png");
	mControlsQuad2 = new JQuad(mControlsTex2,0,0,480,272);
}

void GameStateOptions::Destroy() 
{
	SAFE_DELETE(mControlsQuad1);

	SAFE_DELETE(mControlsTex1);
	
	SAFE_DELETE(mControlsQuad2);

	SAFE_DELETE(mControlsTex2);

	for (int i=0; i<NUMCONFIGS; i++) {
		SAFE_DELETE(mGuiControllers[i])
	}
}

void GameStateOptions::Start()
{
	strcpy(name,"");

	mRenderer->EnableVSync(true);

	movementstyle = ABSOLUTE1;
	char* movement = GetConfig("data/config.txt","movement");
	if (movement != NULL) {
		if (strcmp(movement,"relative") == 0) {
			movementstyle = RELATIVE1;
		}
		else if (strcmp(movement,"absolute") == 0) {
			movementstyle = ABSOLUTE1;
		}
		delete movement;
	}
	mGuiControllers[0]->SetCurr(movementstyle);

	music = OFF;
	char* musicconfig = GetConfig("data/config.txt","music");
	if (musicconfig != NULL) {
		if (strcmp(musicconfig,"on") == 0) {
			music = ON;
		}
		else if (strcmp(musicconfig,"off") == 0) {
			music = OFF;
		}
		delete musicconfig;
	}
	mGuiControllers[1]->SetCurr(music);

	friendlyfire = ON;
	char* ff = GetConfig("data/config.txt","ff");
	if (ff != NULL) {
		if (strcmp(ff,"on") == 0) {
			friendlyfire = ON;
		}
		else if (strcmp(ff,"off") == 0) {
			friendlyfire = OFF;
		}
		delete ff;
	}
	mGuiControllers[2]->SetCurr(friendlyfire);

	menustyle = ANALOG;
	char* menu = GetConfig("data/config.txt","menu");
	if (menu != NULL) {
		if (strcmp(menu,"analog") == 0) {
			menustyle = ANALOG;
		}
		else if (strcmp(menu,"dirpad") == 0) {
			menustyle = DIRPAD;
		}
		delete menu;
	}
	mGuiControllers[3]->SetCurr(menustyle);

	char* nameconfig = GetConfig("data/config.txt","name");
	if (nameconfig != NULL) {
		strncpy(name,nameconfig,15);
		delete nameconfig;
	}
	else {
		strcpy(name,"default");
	}
	name[15] = '\0';

	index = 0;

	mInfoX = -400.0f;
}


void GameStateOptions::End()
{
	mRenderer->EnableVSync(false);
	//delete mGuiControllers[0];
	//mGuiControllers[0] = NULL;
}


void GameStateOptions::Update(float dt)
{
	if (!gDanzeff->mIsActive) {
		
		if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
			mState--;
			if (mState < 0) {
				mState = CONTROLS;
			}
		}
		if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
			mState++;
			if (mState > CONTROLS) {
				mState = 0;
			}
		}
		
		if (mEngine->GetButtonState(PSP_CTRL_RTRIGGER) || mEngine->GetButtonState(PSP_CTRL_LTRIGGER))  {
			for (int i=0; i<NUMCONFIGS; i++) {
				if (mGuiControllers[i] != NULL) {
					mGuiControllers[i]->SetActive(false);
				}
			}
			mGuiControllers[0]->SetCurr(movementstyle);
			mGuiControllers[1]->SetCurr(music);
			mGuiControllers[2]->SetCurr(friendlyfire);
			mGuiControllers[3]->SetCurr(menustyle);

			main = true;
			mInfoX = -400.0f;
		}

	}

	if (mState == GENERAL) {
		mInfoX *= 0.75f;
		if (!gDanzeff->mIsActive) {
			if (main) {
				int temp = index;
				if (mEngine->GetButtonState(PSP_CTRL_UP) || mEngine->GetAnalogY()<64) {
					if (KeyRepeated(PSP_CTRL_UP,dt)) {
						index--;
						if (index == SAVE-1) {
							index--;
						}
						else if (index < 0) {
							index = NUMCONFIGS-1;
						}
					}
				}
				else if (mEngine->GetButtonState(PSP_CTRL_DOWN) || mEngine->GetAnalogY()>192) {
					if (KeyRepeated(PSP_CTRL_DOWN,dt)) {
						index++;
						if (index == SAVE-1) {
							index++;
						}
						else if (index > NUMCONFIGS-1) {
							index = 0;
						}
					}
				}
				else if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					//index = NUMCONFIGS-1;
				}
				else if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					//index = 0;
				}
				else {
					gLastKey = 0;
				}
				if (temp != index) {
					mInfoX = -400.0f;
				}
				else if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
					mState = GAME;
				}
				else if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
					mState = CONTROLS;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
					if (index == 4) {
						main = false;
						gDanzeff->Enable();
						gDanzeff->mString = name;
						tempname = name;
					}
					else if (index == NUMCONFIGS-1) {
						Save();
						mParent->SetNextState(GAME_STATE_MENU);
					}
					/*else if (index == NUMCONFIGS-1) {
						mParent->SetNextState(GAME_STATE_MENU);
					}*/
					if (mGuiControllers[index] != NULL) {
						//mGuiControllers[index]->SetActive(true);
						main = false;
						mGuiControllers[index]->SetActive(true);
					}
				}
				if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
					mParent->SetNextState(GAME_STATE_MENU);
				}
			}
			else {
				if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
					for (int i=0; i<NUMCONFIGS; i++) {
						if (mGuiControllers[i] != NULL) {
							mGuiControllers[i]->SetActive(false);
						}
					}
					mGuiControllers[0]->SetCurr(movementstyle);
					mGuiControllers[1]->SetCurr(music);
					mGuiControllers[2]->SetCurr(friendlyfire);
					mGuiControllers[3]->SetCurr(menustyle);

					main = true;
				}
			}

			for (int i=0; i<NUMCONFIGS; i++) {
				if (mGuiControllers[i] != NULL) {
					if (mGuiControllers[i]->IsActive()) {
						mGuiControllers[i]->Update(dt);
					}
				}
			}

			/**if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
				mParent->SetNextState(GAME_STATE_MENU);
			}
			if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
				mParent->SetNextState(GAME_STATE_MENU);
			}**/
		}
		else if (gDanzeff->mIsActive) {
			gDanzeff->Update(dt);
			if (gDanzeff->mString.length() > 15) {
				gDanzeff->mString = gDanzeff->mString.substr(0,15);
			}
			tempname = (char*)gDanzeff->mString.c_str();
			
			if (mEngine->GetButtonClick(PSP_CTRL_START)) {
				main = true;

				bool valid = true;
				FILE* file = fopen("nataku92.txt","r");
				if (file == NULL) {
					if (stricmp(tempname,"nataku92") == 0 || strstr(tempname,"nataku92") != NULL) {
						valid = false;
					}
				}
				else {
					fclose(file);
				}

				if (valid) {
					strcpy(name,tempname);
				}
				else {
					if (stricmp(name,"nataku92") == 0 || strstr(name,"nataku92") != NULL) {
						strcpy(name,"default");
					}
				}
				gDanzeff->Disable();
			}
			else if (mEngine->GetButtonClick(PSP_CTRL_SELECT)) {
				main = true;
				gDanzeff->Disable();
			}
		}
	}
	
	
	
	
	
	else if (mState == GAME) {
		if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
			mParent->SetNextState(GAME_STATE_MENU);
		}
		if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
			mState == CONTROLS;
		}
		if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
			mState == GENERAL;
		}
		
		if(MusicType == 1) {
			LoadMusic = 1;
			
		}
		else {
			if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
				LoadMusic++;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
				LoadMusic--;
			}
		}
		
		if (LoadMusic > 3) { LoadMusic = 3; }
		if (LoadMusic < 1) { LoadMusic = 1; }
		
	}
	
	
	
	
	
	else if (mState == CONTROLS) {
		if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
			mParent->SetNextState(GAME_STATE_MENU);
		}
		
		if (mControlPage > 2) { mControlPage = 2; }
		if (mControlPage < 1) { mControlPage = 1; }
		
		if(mControlPage == 1) {
			if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
				mControlPage++;
			}
		}
		if(mControlPage == 2) {
			if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
				mControlPage--;
			}
		}
		
		if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
			mState == GENERAL;
		}
		if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
			mState == GAME;
		}
	}
}


void GameStateOptions::Render()
{
	mRenderer->ClearScreen(ARGB(255,255,255,255));
	//mRenderer->FillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,ARGB(255,255,255,255));
	mRenderer->RenderQuad(gBgQuad, 0.0f, 0.0f);

	gFont->SetBase(0);
	gFont->SetScale(0.75f);
	gFont->SetColor(ARGB(255,255,255,255));
	gFont->DrawShadowedString("< [L]",10,10,JGETEXT_LEFT);
	gFont->DrawShadowedString("[R] >",SCREEN_WIDTH-10,10,JGETEXT_RIGHT);
	gFont->SetScale(1.0f);
	
	if (mState == GENERAL) {
		mRenderer->FillRect(0,30,SCREEN_WIDTH,5,ARGB(100,0,0,0));
		mRenderer->FillRect(0,35,SCREEN_WIDTH,175,ARGB(100,0,0,0));
		mRenderer->FillRect(0,35+175,SCREEN_WIDTH,30,ARGB(175,0,0,0));

		mRenderer->FillRect(80,5,98,25,ARGB(175,0,0,0)); // General
		mRenderer->FillRect(200,5,75,25,ARGB(100,0,0,0)); // Game
		mRenderer->FillRect(305,5,92,25,ARGB(100,0,0,0)); // Controlls

		gFont->SetColor(ARGB(255,255,244,53));
		gFont->DrawShadowedString("General",130,10,JGETEXT_CENTER);
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->DrawShadowedString("Game",235,10,JGETEXT_CENTER);
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->DrawShadowedString("Controls",350,10,JGETEXT_CENTER);

		/*gFont->SetColor(ARGB(255,255,255,0));
		gFont->SetScale(1.0f);
		gFont->DrawString("OPTIONS",155,10-2,JGETEXT_CENTER);

		gFont->SetColor(ARGB(255,255,255,255));
		gFont->SetScale(0.75f);
		gFont->DrawString("CONTROLS",325,10,JGETEXT_CENTER);*/

		gFont->SetScale(0.75f);
		gFont->SetColor(ARGB(255,255,255,255));

		if (main) {
			mRenderer->FillRect(0,35+index*25,SCREEN_WIDTH,25,ARGB(255,0,0,0));
			gFont->DrawShadowedString("[X] Select     [O] Cancel and Return to Menu",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
		}
		else {
			mRenderer->FillRect(0,35+index*25,SCREEN_WIDTH,25,ARGB(100,0,128,255));
			if (!gDanzeff->mIsActive) {
				gFont->DrawShadowedString("[DIR PAD/ANALOG] Change Selection    [X] Select    [O] Cancel",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else if (gDanzeff->mIsActive) {
				gFont->DrawShadowedString("[START] Enter    [SELECT] Cancel",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
		}
		mRenderer->DrawLine(160,35,160,35+175,ARGB(255,255,255,255));

		

		gFont->SetScale(1.0f);
		float x = 160-10;
		for (int i=0; i<NUMCONFIGS; i++) {
			if (index == i) {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString(mConfigInfo[i],10.0f+mInfoX,35+175+10);
				gFont->SetScale(1.0f);
				gFont->SetColor(ARGB(255,0,128,255));
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
			}
			gFont->DrawShadowedString(mConfigs[i],x,38+25*i,JGETEXT_RIGHT);
			if (mGuiControllers[i] != NULL) {
				mGuiControllers[i]->Render();
			}

		}
		/*gFont->DrawShadowedString("movement style",x,35,JGETEXT_RIGHT);
		gFont->DrawShadowedString("music",x,60);
		gFont->DrawShadowedString("friendly fire",x,85);
		gFont->DrawShadowedString("name",x,110);
		gFont->DrawShadowedString("save",x,35+(NUMCONFIGS-2)*25);
		gFont->DrawShadowedString("cancel",x,35+(NUMCONFIGS-1)*25);

		gFont->SetBase(128);
		for (int i=0; i<NUMCONFIGS; i++) {
			if (mGuiControllers[i] != NULL) {
				mGuiControllers[i]->Render();
			}
		}*/

		gFont->SetColor(ARGB(255,255,255,255));
		gFont->SetScale(0.75f);
		if (!gDanzeff->mIsActive) {
			gFont->DrawShadowedString(name,160+10,140);
		}
		else if (gDanzeff->mIsActive) {
			gFont->DrawShadowedString(tempname,160+10,140);
			gFont->DrawShadowedString("|",160+10+gFont->GetStringWidth(tempname),140);
			gDanzeff->Render(SCREEN_WIDTH-175,SCREEN_HEIGHT-175);
		}
	}
	
	else if (mState == GAME) {
		mRenderer->FillRect(0,30,SCREEN_WIDTH,5,ARGB(100,0,0,0));
		mRenderer->FillRect(0,35,SCREEN_WIDTH,175,ARGB(100,0,0,0));
		//mRenderer->FillRect(0,35+175,SCREEN_WIDTH,30,ARGB(175,0,0,0)); It's Already Filling

		mRenderer->FillRect(80,5,98,25,ARGB(100,0,0,0)); // General
		mRenderer->FillRect(200,5,75,25,ARGB(175,0,0,0)); // Game
		mRenderer->FillRect(305,5,92,25,ARGB(100,0,0,0)); // Controlls
		
		if(MusicType == 1) {
			LoadMusic = 1;
			mRenderer->FillRect(0,35+175,SCREEN_WIDTH,30,ARGB(200,0,0,0)); // Filling here
		}
		else {
			mRenderer->FillRect(0,35+175,SCREEN_WIDTH,30,ARGB(175,0,0,0)); // ...or here.
		}
		
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->DrawShadowedString("General",130,10,JGETEXT_CENTER);
		gFont->SetColor(ARGB(255,255,244,54));
		gFont->DrawShadowedString("Game",235,10,JGETEXT_CENTER);
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->DrawShadowedString("Controls",350,10,JGETEXT_CENTER);
		
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->SetScale(0.75f);
		gFont->DrawShadowedString("Change Soundtrack to play",SCREEN_WIDTH/2,SCREEN_HEIGHT_F-50-4,JGETEXT_CENTER);
		gFont->SetScale(1.0f);
		gFont->SetColor(ARGB(255,0,128,255));
		mRenderer->FillRect(0,35,SCREEN_WIDTH,25,ARGB(100,0,0,0));
		gFont->DrawShadowedString("Track",150,38*1,JGETEXT_RIGHT);
		if(LoadMusic == 1) {
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Soundtrack 1",170,38*1,JGETEXT_LEFT);
		}
		if(LoadMusic == 2) {
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Soundtrack 2",170,38*1,JGETEXT_LEFT);
		}
		if(LoadMusic == 3) {
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Soundtrack 3",170,38*1,JGETEXT_LEFT);
		}
		
		if(MusicType == 1 || MusicType == 2) {
			// LoadMusic = 1;
			gFont->SetColor(ARGB(255,255,0,0));
			gFont->DrawShadowedString("You can't Change Soundtrack at MusicType = 1",350,38*1,JGETEXT_LEFT);
		}
		
		gFont->SetScale(0.75f);
		gFont->SetColor(ARGB(255,255,255,255));

		gFont->DrawShadowedString("[O] Cancel and Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
		mRenderer->DrawLine(160,35,160,35+175,ARGB(255,255,255,255));
	}
	
	else if (mState == CONTROLS) {
		if(mControlPage == 1) {
			mRenderer->FillRect(0,30,SCREEN_WIDTH,5,ARGB(100,0,0,0));
			mRenderer->FillRect(0,35,SCREEN_WIDTH,205,ARGB(100,0,0,0));
	
			mRenderer->FillRect(80,5,98,25,ARGB(100,0,0,0)); // General
			mRenderer->FillRect(200,5,75,25,ARGB(100,0,0,0)); // Game
			mRenderer->FillRect(305,5,92,25,ARGB(175,0,0,0)); // Controlls
			mRenderer->RenderQuad(mControlsQuad1, 0.0f, 0.0f);
	
	
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("General",130,10,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Game",235,10,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,244,53));
			gFont->DrawShadowedString("Controls",350,10,JGETEXT_CENTER);
			
			gFont->SetScale(0.75f);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("[Down] Page 2    [O] Cancel and Return to Menu    (In Game)",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
	
			/*gFont->SetColor(ARGB(255,255,255,0));
			gFont->SetScale(1.0f);
			gFont->DrawString("CONTROLS",325,10-2,JGETEXT_CENTER);
	
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->SetScale(0.75f);
			gFont->DrawString("OPTIONS",155,10,JGETEXT_CENTER);*/
		}
		
		if(mControlPage == 2) {
			mRenderer->FillRect(0,30,SCREEN_WIDTH,5,ARGB(100,0,0,0));
			mRenderer->FillRect(0,35,SCREEN_WIDTH,205,ARGB(100,0,0,0));
	
			mRenderer->FillRect(80,5,98,25,ARGB(100,0,0,0)); // General
			mRenderer->FillRect(200,5,75,25,ARGB(100,0,0,0)); // Game
			mRenderer->FillRect(305,5,92,25,ARGB(175,0,0,0)); // Controlls
			mRenderer->RenderQuad(mControlsQuad2, 0.0f, 0.0f);
			
	
	
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("General",130,10,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Game",235,10,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,244,53));
			gFont->DrawShadowedString("Controls",350,10,JGETEXT_CENTER);
			
			gFont->SetScale(0.75f);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("[Up] Page 1    [O] Cancel and Return to Menu    (Spectator)",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
	
		}
	}
}

void GameStateOptions::Save()
{
	FILE *file;
	file = fopen("data/config.txt", "w"); 
	if (file == NULL) return;
	
	if (movementstyle == RELATIVE1) {
		fputs("movement = relative\r\n",file);
	}
	else if (movementstyle == ABSOLUTE1) {
		fputs("movement = absolute\r\n",file);	
	}

	if (music == ON) {
		fputs("music = on\r\n",file);
	}
	else if (music == OFF) {
		fputs("music = off\r\n",file);	
	}

	if (friendlyfire == ON) {
		fputs("ff = on\r\n",file);
	}
	else if (friendlyfire == OFF) {
		fputs("ff = off\r\n",file);	
	}

	if (menustyle == ANALOG) {
		fputs("menu = analog\r\n",file);
	}
	else if (menustyle == DIRPAD) {
		fputs("menu = dirpad\r\n",file);	
	}

	fputs("name = ",file);
	fputs(name,file);
	fputs("\r\n",file);

	fputs("map = gunstest\r\n",file);
	
	if(LoadMusic == 1) {
		fputs("LoadMusic = 1\r\n",file);
		fputs("Old_Music = 1", file);
	}
	if(LoadMusic == 2) {
		fputs("LoadMusic = 2\r\n",file);
		fputs("Old_Music = 2", file);
	}
	if(LoadMusic == 3) {
		fputs("LoadMusic = 3\r\n",file);
		fputs("Old_Music = 3", file);
	}
	
	fclose(file);
}

void GameStateOptions::ButtonPressed(int controllerId, int controlId)
{
	main = true;
	mGuiControllers[controllerId]->SetActive(false);
	switch (controllerId)
	{
	case 0:
		movementstyle = controlId;
		//mGuiControllers[0]->mDefault = mGuiControllers[0]->GetCurrentIndex();
		//break;
		break;
	case 1:
		music = controlId;
		//ResetRound();
		//mGuiController->SetActive(false);
		break;
	case 2:
		friendlyfire = controlId;
		//End();
		//mParent->SetNextState(GAME_STATE_MENU);
		break;
	case 3:
		menustyle = controlId;
		//mEngine->End();
		break;
	}

	
}

