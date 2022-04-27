#ifndef _GAME_SNAKE_H_
#define _GAME_SNAKE_H_

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

#include "Manettes.h"
#include "Text.h"
#include "Game.h"

/*********************************************************************************************/
/*  Principe du Jeu:                                                                         */
/*    -Ce jeu deplace un plateau de 6 pixels, pour faire rebondir un pixel                   */
/*  But:                                                                                     */
/*    -TODO                                                                                  */
/*                                                                                           */
/*                                                                                           */
/*********************************************************************************************/


class Game_Snake : public Game 
{
    public:    
    Game_Snake(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
    {
        //recuperation du display manager
        m_pDisplayManager = pDisplayManager; 
        m_pAudio = pAudio; 
      
        m_nLive = 0;    
    }
    
    void Load(unsigned int nLvl)
    {
    }
        
    void Init(void)
    {         
        Load(0); 
    }
    
    void nextLevel(void)
    {    
    }
    
    void Draw(void)
    {
        m_pDisplayManager->OFF();
    }
    
    void Update(StructManettes DataManettes)
    {    
    }
        
    void UpdateGame(StructManettes DataManettes)
    { 
    }

    private:    
    DisplayManager* m_pDisplayManager;  //Poiteur sur le tableau d'affichage (table led) (forme matricielle)
    MyDFPlayer* m_pAudio; 
    int m_nLive;
    
};

#endif
