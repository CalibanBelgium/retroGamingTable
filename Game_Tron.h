#ifndef _GAME_TRON_H_
#define _GAME_TRON_H_

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

#define MaxLastPositions (SIZE_ALLSTRIP_X * SIZE_ALLSTRIP_Y)
#define HalfMaxLastPositions (MaxLastPositions/2)

class Game_Tron : public Game 
{
    public:    
    Game_Tron(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
    {
        //recuperation du display manager
        m_pDisplayManager = pDisplayManager; 
        m_pAudio = pAudio; 
      
        m_P1_LastPositions = new Vect2D[HalfMaxLastPositions];
        m_P2_LastPositions = new Vect2D[HalfMaxLastPositions];
        
        Init();
    }
    ~Game_Tron()
    {
        delete[] m_P1_LastPositions;
        delete[] m_P2_LastPositions;
    }
    
    void Load(unsigned int nLvl)
    {      
    }
        
    void Init(void)
    {       
        m_nbrPosition = 0;   
        m_P1_Dead = false;
        m_P2_Dead = false;
        m_P1_Positions.X = SIZE_ALLSTRIP_X/2;
        m_P1_Positions.Y = SIZE_ALLSTRIP_Y/4;
        m_P2_Positions.X = SIZE_ALLSTRIP_X/2-1;
        m_P2_Positions.Y = 3*SIZE_ALLSTRIP_Y/4;
        
        m_P1_Direction.X = -1;m_P1_Direction.Y = 0;
        m_P2_Direction.X = +1;m_P2_Direction.Y = 0;
        
        m_nTime_LastUpdate = 0;
        m_nTimeState = 0;
        
        Vect2D zero; zero.X = 0; zero.Y = 0;
        for(unsigned int ii = 0; ii < HalfMaxLastPositions; ++ii)
        {
            m_P1_LastPositions[ii] = zero;
            m_P1_LastPositions[ii] = zero;
        }
        m_State = ENUM_State_WaitStart;
        m_State_Next = ENUM_State_WaitStart;
        m_State_Last = ENUM_State_WaitStart;
    }
    
    void nextLevel(void)
    {    
    }
    
    void Draw(void)
    {
        m_pDisplayManager->OFF();
        
        //on onne les murs
        for(int ii = 0; ii < SIZE_ALLSTRIP_X; ++ii)
        {
            m_pDisplayManager->SetPixel(ii, 0, C_WHITE );
            m_pDisplayManager->SetPixel(ii, SIZE_ALLSTRIP_X-1, C_WHITE );
        }
        for(int ii = 0; ii < SIZE_ALLSTRIP_Y; ++ii)
        {
            m_pDisplayManager->SetPixel(0, ii, C_WHITE );
            m_pDisplayManager->SetPixel(SIZE_ALLSTRIP_X-1, ii, C_WHITE );
        }

        //old possition
        for(int ii = 0; ii < m_nbrPosition; ++ii)
        {
            m_pDisplayManager->SetPixel(m_P1_LastPositions[ii], C_TURQUOISE );
            m_pDisplayManager->SetPixel(m_P2_LastPositions[ii], C_ORANGE_dark );
        }
        m_pDisplayManager->SetPixel(m_P1_Positions, C_BLEU_steel );
        m_pDisplayManager->SetPixel(m_P2_Positions, C_ORANGE_red );
    }
      
    void Update(StructManettes DataManettes)
    {
        m_State_Last = m_State;
        m_State = m_State_Next;        
        
        switch(m_State)
        {
            case ENUM_State_WaitStart : WaitStart(DataManettes) ; break;
            case ENUM_State_Start : UpdateGame(DataManettes) ; break;
            case ENUM_State_Fini : Fini(DataManettes); break; 
        }
    }
    void WaitStart(StructManettes DataManettes)
    {      
        if(DataManettes.DataManette1 & BTN_START) m_State_Next = ENUM_State_Start;
        if(DataManettes.DataManette2 & BTN_START) m_State_Next = ENUM_State_Start;
        
        m_pDisplayManager->OFF();
        Text txt0("PRESS");
        txt0.Draw({3,8}, C_GREEN, m_pDisplayManager); 
        Text txt1("START");
        txt1.Draw({4,14}, C_GREEN, m_pDisplayManager); 
    }  
    void UpdateGame(StructManettes DataManettes)
    {        
        //update des positions        
        if(DataManettes.DataManette1 & BTN_LEFT 
          && !(m_P1_Direction.X == 1 && m_P1_Direction.Y == 0))       {m_P1_Direction.X = -1;m_P1_Direction.Y = 0;}
        else if(DataManettes.DataManette1 & BTN_RIGHT 
          && !(m_P1_Direction.X == -1 && m_P1_Direction.Y == 0))      {m_P1_Direction.X = +1;m_P1_Direction.Y = 0;}
        else if(DataManettes.DataManette1 & BTN_UP 
          && !(m_P1_Direction.X == 0 && m_P1_Direction.Y == 1))       {m_P1_Direction.X = 0; m_P1_Direction.Y = -1;}
        else if(DataManettes.DataManette1 & BTN_DOWN 
          && !(m_P1_Direction.X == 0 && m_P1_Direction.Y == -1))      {m_P1_Direction.X = 0; m_P1_Direction.Y = +1;}
        
        if(DataManettes.DataManette2 & BTN_LEFT 
          && !(m_P2_Direction.X == 1 && m_P2_Direction.Y == 0))       {m_P2_Direction.X = -1;m_P2_Direction.Y = 0;}
        else if(DataManettes.DataManette2 & BTN_RIGHT 
          && !(m_P2_Direction.X == -1 && m_P2_Direction.Y == 0))      {m_P2_Direction.X = +1;m_P2_Direction.Y = 0;}
        else if(DataManettes.DataManette2 & BTN_UP 
          && !(m_P2_Direction.X == 0 && m_P2_Direction.Y == 1))       {m_P2_Direction.X = 0; m_P2_Direction.Y = -1;}
        else if(DataManettes.DataManette2 & BTN_DOWN 
          && !(m_P2_Direction.X == 0 && m_P2_Direction.Y == -1))      {m_P2_Direction.X = 0; m_P2_Direction.Y = +1;}
        
        if(millis() - m_nTime_LastUpdate < 20) return;
      
        m_nTime_LastUpdate = millis();
      
        //update
        m_P1_LastPositions[m_nbrPosition] = m_P1_Positions;
        m_P2_LastPositions[m_nbrPosition] = m_P2_Positions;
        ++m_nbrPosition;        
        
        m_P1_Positions.X += m_P1_Direction.X;
        m_P1_Positions.Y += m_P1_Direction.Y;
        m_P2_Positions.X += m_P2_Direction.X;
        m_P2_Positions.Y += m_P2_Direction.Y;

        //test impact mur
        if(m_P1_Positions.X <= 0 || m_P1_Positions.X >= SIZE_ALLSTRIP_X-1 || m_P1_Positions.Y <= 0 || m_P1_Positions.Y >= SIZE_ALLSTRIP_Y-1) m_P1_Dead = true;
        if(m_P2_Positions.X <= 0 || m_P2_Positions.X >= SIZE_ALLSTRIP_X-1 || m_P2_Positions.Y <= 0 || m_P2_Positions.Y >= SIZE_ALLSTRIP_Y-1) m_P2_Dead = true;

        //test impact other player
        for (int ii = 0; ii < m_nbrPosition; ++ii)
        {
            if((m_P2_LastPositions[ii].X == m_P1_Positions.X && m_P2_LastPositions[ii].Y == m_P1_Positions.Y)  
              || (m_P1_LastPositions[ii].X == m_P1_Positions.X && m_P1_LastPositions[ii].Y == m_P1_Positions.Y))
              m_P1_Dead = true;
            if((m_P1_LastPositions[ii].X == m_P2_Positions.X && m_P1_LastPositions[ii].Y == m_P2_Positions.Y) 
              || (m_P2_LastPositions[ii].X == m_P2_Positions.X && m_P2_LastPositions[ii].Y == m_P2_Positions.Y) )
              m_P2_Dead = true;
        }

        if(m_P1_Positions.X == m_P2_Positions.X && m_P1_Positions.Y == m_P2_Positions.Y)  {m_P2_Dead = true;m_P1_Dead = true;}
        
        
        if(m_P1_Dead || m_P2_Dead) m_State_Next = ENUM_State_Fini;
        Draw(); 
    }
    void Dead(StructManettes DataManettes){ //NONE
    }
    void Fini(StructManettes DataManettes)    
    {      
        Draw();   
        if(m_State_Last != m_State) 
        { 
            m_nTimeState = millis();          
        }
        else
        {          
            if(millis() - m_nTimeState > 1000) 
            {
              m_State_Next = ENUM_State_WaitStart;
              Init();
            }
            else
            {
                if( m_P1_Dead && m_P2_Dead)
                {
                    Text txt0("NO");
                    txt0.Draw({10,10}, C_GREEN, m_pDisplayManager);
                    Text txt1("WIN");
                    txt1.Draw({10,16}, C_GREEN, m_pDisplayManager);      
                }
                else if( m_P1_Dead)
                {
                    Text txt0("RED");
                    txt0.Draw({10,10}, C_GREEN, m_pDisplayManager);
                    Text txt1("WIN");
                    txt1.Draw({10,16}, C_GREEN, m_pDisplayManager);      
                }
                else if( m_P2_Dead)
                {
                    Text txt0("BLUE");
                    txt0.Draw({10,10}, C_GREEN, m_pDisplayManager);
                    Text txt1("WIN");
                    txt1.Draw({10,16}, C_GREEN, m_pDisplayManager);      
                }
            }
        } 
    }
    
    private:    
    DisplayManager* m_pDisplayManager;  //Poiteur sur le tableau d'affichage (table led) (forme matricielle)
    MyDFPlayer* m_pAudio; 
    unsigned long m_nTimeState; 
    unsigned long m_nTime_LastUpdate; 

    int m_nbrPosition;
    Vect2D *m_P1_LastPositions, *m_P2_LastPositions;
    Vect2D m_P1_Positions, m_P2_Positions;
    Vect2D m_P1_Direction, m_P2_Direction;
    bool m_P1_Dead, m_P2_Dead;
    
};

#endif
