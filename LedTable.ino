//Project Table LED 30 LEDs par 30 LEDs 

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

#include "Config.h"
#include "MyDFPlayer.h"
#include "DisplayManager.h"
#include "Manettes.h"
#include "Text.h"

#include "Game.h"
#include "Game_CasseBrique.h"
#include "Game_Pong.h"
#include "Game_Tetrix.h"
#include "Game_Snake.h"
#include "Game_Tron.h"
#include "Time.h"

//Objects globaux
DisplayManager* g_pDisplayManager = NULL;
Manettes* g_pManettes = NULL;
MyDFPlayer* g_pAudio = NULL;
Game* g_pGame = NULL;

//variable Global
GameType g_ModeSelected = ENUM_GAME_NONE;
int g_nSelectionMenu = 0;
bool g_bMenuQuite = false;
int g_nSelectionMenuQuite = 0;
Text g_TextMenu_Title = Text("MENU");
Text g_TextMenu[ENUM_GAME_SIZE] = { Text("CASE_BRIQUE"), Text("PONG"), Text("TETRIS"),Text("TIME"),Text("TRON")};
Text g_TextMenuPause_Title = Text("PAUSE");
Text g_TextMenuPause[2] = {Text("RESUME"), Text("EXIT")};
Text g_Fleche = Text("-");


//Fonctions
void MenuPrincipal(StructManettes DataManettes);
void MenuQuite(StructManettes DataManettes);


/************************************************************************/
void setup() 
{
    delay(100); 
#ifdef SerialDebug
Serial.begin(9600);
Serial.println("INIT START ....");
//Print_freeRam(); 
#endif
    Wire.begin();

    //Init Audio
#ifdef SerialDebug
Serial.println("Initialization Audio ...");
#endif  
    g_pAudio = new MyDFPlayer();
    g_pAudio->Reset();
    g_pAudio->SetVolume(30);
    
    //init SD Card
#ifdef SerialDebug
Serial.println("Initialization SD card...");
#endif  
    if (!SD.begin(PIN_SDCARD)) 
    {
#ifdef SerialDebug
Serial.println("initialization failed!");
#endif
        return;
    }
    //Init DisplayManager
#ifdef SerialDebug
Serial.println("---> INIT Display Manager");
#endif
    g_pDisplayManager = new DisplayManager(SIZE_ALLSTRIP_X, SIZE_ALLSTRIP_Y);

    //Init Manettes
#ifdef SerialDebug
//Print_freeRam(); 
Serial.println("---> INIT Manette");
#endif
    g_pManettes = new Manettes(PIN_LATCH, PIN_CLOCK, PIN_DATAMANETTE_1, PIN_DATAMANETTE_2);
    
    //init random
#ifdef SerialDebug
Serial.println("---> INIT Random");
#endif   
    byte second_, minute_, hour_, dayOfWeek_, dayOfMonth_, month_, year_;
    Time::readDS3231time(second_, minute_, hour_, dayOfWeek_, dayOfMonth_, month_, year_);    
    int nSeed = (second_+ (minute_)*hour_)/dayOfWeek_ + month_; //oui je m'amuse bien ^^
    randomSeed(nSeed);

#ifdef SerialDebug
//Print_freeRam(); 
Serial.println("INIT END");
#endif
}

void loop() 
{  
#ifdef SerialDebug
Serial.println("LOOP");
#endif

    //Update des manettes
    g_pManettes->Update();
    StructManettes DataManettes = g_pManettes->GetData();
    
    //Update 
    if(g_ModeSelected == ENUM_GAME_NONE) 
    {    
        MenuPrincipal(DataManettes);
    }
    else
    {      
        if(DataManettes.DataManette1 & BTN_SELECT || DataManettes.DataManette2 & BTN_SELECT)
        {
          g_bMenuQuite = true;
          g_nSelectionMenuQuite = 0;
        }
        
        if(g_bMenuQuite)
        {   
            MenuQuite(DataManettes);
        }
        else
        {
            g_pGame->Update(DataManettes);
        }
    }

    //Update de l'affichage
    g_pDisplayManager->Draw();
    delay(10);
}

void MenuQuite(StructManettes DataManettes)
{
    if(DataManettes.DataManette1 & BTN_START || DataManettes.DataManette2 & BTN_START)
    {
        if(g_nSelectionMenuQuite == 0){}      //RESUME
        else if (g_nSelectionMenuQuite == 1)  //EXIT
        {
            //destruction du jeu en cour
            delete g_pGame;
            g_ModeSelected = ENUM_GAME_NONE;
        }        
        g_bMenuQuite = false;
        delay(500);
    }
    else if(DataManettes.DataManette1 & BTN_UP || DataManettes.DataManette2 & BTN_UP)
    {
        g_pAudio->SetVolume(25);  
        g_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_Menu); 
        g_nSelectionMenuQuite = 0;
        delay(50);
    }
    else if(DataManettes.DataManette1 & BTN_DOWN || DataManettes.DataManette2 & BTN_DOWN)
    {
        g_pAudio->SetVolume(25);  
        g_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_Menu); 
        g_nSelectionMenuQuite = 1;
        delay(50);
    }
    
    g_pDisplayManager->OFF();
    
    g_TextMenuPause_Title.Draw({3,3}, C_BLUE, g_pDisplayManager);  
    g_TextMenuPause[0].Draw({5,10}, C_GREEN, g_pDisplayManager);  
    g_TextMenuPause[1].Draw({5,16}, C_GREEN, g_pDisplayManager);      
    g_Fleche.Draw({1,10 + g_nSelectionMenuQuite * 6  }, C_RED, g_pDisplayManager);

}

void MenuPrincipal(StructManettes DataManettes)
{  
  Serial.println("MenuPrincipal");
    if(DataManettes.DataManette1 & BTN_START || DataManettes.DataManette2 & BTN_START)
    {
        g_pAudio->SetVolume(25);  
        g_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_Menu); 
        g_ModeSelected = (GameType)(g_nSelectionMenu);
        switch(g_ModeSelected)
        {
            case ENUM_GAME_CASSEBRIQUE:
                {g_pGame = new Game_CasseBrique(g_pDisplayManager, g_pAudio); g_pGame->Init(); break;}
            case ENUM_GAME_PONG:
                {g_pGame = new Game_Pong(g_pDisplayManager, g_pAudio);g_pGame->Init(); break;}
            case ENUM_GAME_TETRIX:
                {g_pGame = new Game_Tetrix(g_pDisplayManager, g_pAudio);g_pGame->Init(); break;}
            case ENUM_GAME_TRON:
                {g_pGame = new Game_Tron(g_pDisplayManager, g_pAudio);g_pGame->Init(); break;}
            case ENUM_GAME_TIME:
                {g_pGame = new Time(g_pDisplayManager, g_pAudio); break;}
        }
    }
    else if(DataManettes.DataManette1 & BTN_UP || DataManettes.DataManette2 & BTN_UP)
    {
        g_pAudio->SetVolume(25);  
        g_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_Menu); 
        --g_nSelectionMenu;
        g_nSelectionMenu = (g_nSelectionMenu + ENUM_GAME_SIZE)%ENUM_GAME_SIZE;
        delay(50);
    }
    else if(DataManettes.DataManette1 & BTN_DOWN || DataManettes.DataManette2 & BTN_DOWN)
    {
        g_pAudio->SetVolume(25);  
        g_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_Menu); 
        ++g_nSelectionMenu;
        g_nSelectionMenu = (g_nSelectionMenu + ENUM_GAME_SIZE)%ENUM_GAME_SIZE;
        delay(50);
    }
    
    g_pDisplayManager->OFF();
    
    g_TextMenu_Title.Draw({5,2}, C_BLUE, g_pDisplayManager);     
    g_TextMenu[(g_nSelectionMenu-1+ENUM_GAME_SIZE)%ENUM_GAME_SIZE].Draw({5, 9}, C_GREEN, g_pDisplayManager); 
    g_TextMenu[(g_nSelectionMenu  +ENUM_GAME_SIZE)%ENUM_GAME_SIZE].Draw({5,16}, C_GREEN, g_pDisplayManager); 
    g_TextMenu[(g_nSelectionMenu+1+ENUM_GAME_SIZE)%ENUM_GAME_SIZE].Draw({5,23}, C_GREEN, g_pDisplayManager);  
    g_Fleche.Draw({1,16}, C_RED, g_pDisplayManager);

}

/*
void SequenceTest(void)
{    
    //init a zero
    m_pDisplayManager->OFF();
    
    //init variables de la fonction
    ColorRGB Black = ColorRGB{0,0,0};
    int nSizeMax = min(SIZE_ALLSTRIP_X, SIZE_ALLSTRIP_Y);

    //premiere sequence //diagonale
    for(unsigned int ii = 0; ii < 30; ++ii)
    {
Serial.println("diagonale");
        m_pDisplayManager->OFF();
        for(unsigned int xx = 0; xx < SIZE_ALLSTRIP_X ; ++xx)
        {
            for(unsigned int yy = 0; yy < SIZE_ALLSTRIP_Y ; ++yy)
            {
                if((yy+xx+ii)%4 == 0) m_pDisplayManager->SetPixel(xx,yy,ColorRGB{100,50,100}); // on pourra jouer avec la couleur en fonction de la position dans la matrice
            }
        }
        //affichage
        m_pDisplayManager->Draw();
        delay(100);
    }
    
    //next sequence //Horizontal on joue avec les couleurs
    for(unsigned int ii = 0; ii < 30; ++ii)
    {
Serial.println("Horizontal");
        m_pDisplayManager->OFF();
        for(unsigned int xx = 0; xx < SIZE_ALLSTRIP_X ; ++xx)
        {
            for(unsigned int yy = ii%4; yy < SIZE_ALLSTRIP_Y ; yy+=4)
            {
                m_pDisplayManager->SetPixel(xx, yy, Wheel((int)((xx+yy+ii)*(255.0f/60.0f)) & 255)); // on pourra jouer avec la couleur en fonction de la position dans la matrice                
            }
        }
        //affichage
        m_pDisplayManager->Draw();
        delay(100);
    }
    
    //next sequence //Carre
    for(unsigned int ii = 0; ii < 30; ++ii)
    {
Serial.println("Carre");
        m_pDisplayManager->OFF();
        for(unsigned int jj = 0; jj < nSizeMax/2 ; ++jj)
        {
            if((jj+ii)%4 == 0)
            {
                for(unsigned int kk = 0; kk < nSizeMax-jj*2 ; ++kk)
                {
                    m_pDisplayManager->SetPixel(jj+kk, jj,           ColorRGB{60,50,100}); // on pourra jouer avec la couleur en fonction de la position dans la matrice   
                    m_pDisplayManager->SetPixel(jj+kk, nSizeMax-jj,  ColorRGB{60,50,100}); // on pourra jouer avec la couleur en fonction de la position dans la matrice      
                    m_pDisplayManager->SetPixel(jj, jj+kk,           ColorRGB{60,50,100}); // on pourra jouer avec la couleur en fonction de la position dans la matrice   
                    m_pDisplayManager->SetPixel(nSizeMax-jj, jj+kk,  ColorRGB{60,50,100}); // on pourra jouer avec la couleur en fonction de la position dans la matrice                  
                }
            }
        }
        //affichage
        m_pDisplayManager->Draw();
        delay(100);
    }
    //next sequence //trigo
    for(unsigned int ii = 0; ii < 30; ++ii)
    {
Serial.println("trigo");
        m_pDisplayManager->OFF();//init a zero
        for(unsigned int xx = 0; xx < SIZE_ALLSTRIP_X ; ++xx)
        {
            int nCos = (int)(cos((float)(xx+ii)/5.0f)*10.0f)+15;//cos ->variation de -1 a +1
            int nSin = (int)(sin((float)(xx+ii)/5.0f)*10.0f)+15;//sin ->variation de -1 a +1
            m_pDisplayManager->SetPixel(xx, nCos,      ColorRGB{250,0,0}); // on pourra jouer avec la couleur en fonction de la position dans la matrice 
            m_pDisplayManager->SetPixel(xx, nSin,      ColorRGB{0,250,0}); // on pourra jouer avec la couleur en fonction de la position dans la matrice 
            m_pDisplayManager->SetPixel(nCos, nSin,    ColorRGB{0,0,250}); // on pourra jouer avec la couleur en fonction de la position dans la matrice   
        }
        //affichage
        m_pDisplayManager->Draw();
        delay(100);
    }
}*/

