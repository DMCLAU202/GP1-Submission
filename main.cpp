#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //It is essential that this is declared so that the local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cBkGround.h"
#include "cSprite.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "asteroidsGame.h"
#include "cButton.h"
#include "cFileHandler.h"
int score = 0;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Window settings are set up
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	// This code attaches our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// This code attaches the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//with this code it attempts to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If the window fails to be created

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); // This resets the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}
	int score = 0;
	// The code below clears key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);


	/* loads random seed: */
	srand((unsigned int)time(NULL));

	// This code makes vector array of textures
	LPCSTR texturesToUse[] = { "Images\\zombie.png", "Images\\zombie.png", "Images\\enemy character.png", "Images\\enemy character.png", "Images\\Bullet1.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// This code makes vector array of background textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/Main level.jpg", "Images/Start menu.jpg", "Images/End scene.jpg" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());
	cTexture textureBkgd;

	// Generates vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exit button.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/start button.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton saveButton;
	saveButton.attachInputMgr(theInputMgr);
	saveButton.setTexture(btnTextureList[4]->getTexture());
	saveButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	// loads game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Trance.wav", "Audio/Hlaser.wav", "Audio/Explosion.wav" };
	// first [0] is background theme
	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);


	// loads game fonts
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/bakery.otf", "Fonts/Wasted.otf" };

	theFontMgr->addFont("Baked", gameFonts[0], 24);
	theFontMgr->addFont("Funk", gameFonts[1], 24);

	// Generates vector for enemy objects.

	for (int astro = 0; astro < 50; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(800.0f / (rand() % 5 + 1), 600.0f / (rand() % 5 + 1)));    // (glm::vec2(800.0f / (rand() % 5 + 1), 600.0f / (rand() % 5 + 1)))
		theAsteroids[astro]->setSpriteTranslation(glm::vec2(5.0f, 5.0f));  // (glm::vec2((200.0f / 5.0f), (rand() % 4 + 1)))
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}
	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\Main player character.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attaches the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(512.0f, 400.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attaches sound manager to main player character sprite
	rocketSprite.attachSoundMgr(theSoundMgr);


	// include an exit button

	cFileHandler theFile("Data/usermap.dat");
	string mapData;

	string outputMsg;
	string strMsg[] = { "A = left, D = right, Space = shoot","Survive the Zombie Horde!", "Zombie Shooter", "Your a fierce Zombie Hunter!","Can you do it again?" };

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	//This is the mainloop, frames keep rendering until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events

									   //We get the amount of time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState) //switch statement to control the state of the game
		{
		case MENU:
			// Generates the menu game state (menu screen)
			spriteStartBkgd.render();
			//places buttons and renders them
			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();
			//If the start button is pressed change the gamer to the playing screen
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(elapsedTime);

			// Show lines "Survive the Zombie Horde!"
			outputMsg = strMsg[2];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[1];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;
		case PLAYING:
			//generate the playing state (main game)
			spriteBkgd.render();
			{
				//generate the enemies on screen 
				vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
				while (asteroidIterator != theAsteroids.end())
				{
					if ((*asteroidIterator)->isActive() == false)
					{
						//destroy enemy sprite and increase score by 1.
						asteroidIterator = theAsteroids.erase(asteroidIterator);
						score++;
					}
					else
					{
						(*asteroidIterator)->update(elapsedTime);
						(*asteroidIterator)->render();
						++asteroidIterator;
					}
				}
			}
			//theEnmemies[0]->render();
			rocketSprite.update(elapsedTime);
			rocketSprite.update(elapsedTime);
			rocketSprite.render();
			// draw Score and score number
			score = rocketSprite.updater(score);
			theFontMgr->getFont("Funk")->printText("Zombie Shooter", FTPoint(400.0f, 350.0f, 0.0f));
			theFontMgr->getFont("Funk")->printText("Score", FTPoint(200.0f, 750.0f, 0.0f));
			outputMsg = to_string(score);
			theFontMgr->getFont("Funk")->printText(outputMsg.c_str(), FTPoint(350.0f, 750.0, 0.0f));
			// render button and reset clicked to false;
			exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);
			outputMsg = strMsg[2];
			theFontMgr->getFont("Funk")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

			break;


		case END:
			//stop game state (game over screen)
			spriteEndBkgd.render();

			playButton.setClicked(false);
			exitButton.setClicked(false);
			// display the start button and exit button
			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			playButton.render();
			exitButton.render();
			// if start button is pressed, return to play screen
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(theGameState, END);
			//show end screen text messages
			outputMsg = strMsg[2];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[4];
			theFontMgr->getFont("Baked")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;

		}

		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

	return 0; //Return success

}
