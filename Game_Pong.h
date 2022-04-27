#ifndef _GAME_PONG_H_
#define _GAME_PONG_H_

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
#define DELAY_STATE_FIN  (1000)
#define DELAY_STATE_NEWLIVE (1000)
//#define CASSEBRIQUE_PATH_LVL ("Game/CasseB/lvl_")//suivi du numereau de niveau et de l'extention '.txt'

class Game_Pong : public Game 
{
    public:    
    Game_Pong(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
    {
        //recuperation du display manager
        m_pDisplayManager = pDisplayManager; 
        m_pAudio = pAudio; 

        //reset de mur
        m_nMurSize = 0;
        m_Murs = NULL;
                        
        //reset de la ball
        m_Ball.Size = {0,0};
        m_Ball.Vitesse = {0,0};
        m_Ball.CUL = {0,0};
        m_Ball.Color = C_YELLOW;
        
        //reset de la bat
        m_Bat1.Size = {0,0};
        m_Bat1.CUL = {0,0};
        m_Bat1.Color = C_BLUE;

        m_Bat2.Size = {0,0};
        m_Bat2.CUL = {0,0};
        m_Bat2.Color = C_RED;

        //game
        m_State = ENUM_State_WaitStart;
        m_State_Last = m_State ;
        m_State_Next = m_State ;
        m_nTimeState = 0;
        m_bWin = false;

        m_nLivePl1 = 0;  
        m_nLivePl2 = 0;   
        m_nLevel = 0;
    }
    
    void Load(unsigned int nLvl)
    {
        //ResetBrique();
        ResetMur();
             
        //lecture sur card sd
        //creation du niveau
                
        //Load test
        //init les murs
        m_nMurSize = 30+30;
        m_Murs = new Mur[m_nMurSize];
        for(int ii = 0; ii < 30; ++ii)
        {
            m_Murs[ii].Center = {0,ii};
            m_Murs[ii+30].Center = {29,ii};
            m_Murs[ii].Color = Adafruit_NeoPixel::Color(150,150,150);
            m_Murs[ii+30].Color = Adafruit_NeoPixel::Color(150,150,150);
        }

        //init la ball
        float xx=0.00;
        float yy=0.00;
        int x=0;
        int y=0;
        xx=random(50,90);
        xx=xx/100;
        yy=random(50,90);
        yy=yy/100;
        x=random(2);
        y=random(2);
        if(x==1) m_Ball.Vitesse.X = xx;
        if(x==0) m_Ball.Vitesse.X = -xx;
        if(y==1) m_Ball.Vitesse.Y = yy;
        if(y==0) m_Ball.Vitesse.Y = -yy;
        //m_Ball.Vitesse.X = 0.75;//0.3;
        //m_Ball.Vitesse.Y = -0.8;//-0.3; 

              
        m_Ball.CUL = {14,14}; 
        m_Ball.Size = {1,1};

        m_Ball.Color = C_YELLOW;
        
        //init la bat
        m_Bat1.Size = {6,1}; 
        m_Bat1.CUL = {14,29};
        m_Bat1.Color = C_BLUE;
        
        m_Bat2.Size = {6,1}; 
        m_Bat2.CUL = {14,0};
        m_Bat2.Color = C_RED;
        
        m_ColorLivePl1 = C_BLUE;
        m_ColorLivePl2 = C_RED;
        
    }
    
    void Init(void)
    {         
        Load(0);             

        //init info partie
        m_nNbrCoup = 0;
        m_State = ENUM_State_WaitStart;
        m_State_Last = m_State ;
        m_nTimeState = 0;
        m_bWin = false;
        m_nLivePl1 = 10;   
        m_nLivePl2 = 10;  
        m_nLivePl1Old = m_nLivePl1;   
        m_nLivePl2Old = m_nLivePl2; 
         
    }
    
    void nextLevel(void){}; //Not used
    
    void Draw(void)
    {
        m_pDisplayManager->OFF();
        
        //on donne au displaymanager les type pour chaque pixels        
        //on donne les murs
        for(int ii = 0; ii < m_nMurSize; ++ii)//on onne les murs
        {
            m_pDisplayManager->SetPixel(m_Murs[ii].Center, m_Murs[ii].Color );
        }
                    
        //on donne la balle   
        for(int xx = 0; xx < m_Ball.Size.X; ++xx)
            for(int yy = 0; yy < m_Ball.Size.Y; ++yy)
            {
              m_pDisplayManager->SetPixel(m_Ball.CUL.X+xx, m_Ball.CUL.Y+yy , m_Ball.Color );
            }
        
        //on donne la bat        
        for(int ii = -(m_Bat1.Size.X+1)/2; ii < m_Bat1.Size.X/2; ++ii)
        {
            m_pDisplayManager->SetPixel(m_Bat1.CUL.X + ii, m_Bat1.CUL.Y, m_Bat1.Color );
        }    

        for(int ii = -(m_Bat2.Size.X+1)/2; ii < m_Bat2.Size.X/2; ++ii)
        {
            m_pDisplayManager->SetPixel(m_Bat2.CUL.X + ii, m_Bat2.CUL.Y, m_Bat2.Color );
        }  
        //on donne les Lives
        for(int ii = 0; ii < m_nLivePl1; ++ii)
        {
          
          m_pDisplayManager->SetPixel(29, 29-ii, m_ColorLivePl1);
          
        }
                for(int ii = 0; ii < m_nLivePl2; ++ii)
        {
          
          
          m_pDisplayManager->SetPixel(00, 00+ii, m_ColorLivePl2);
        }
    }
  
    void Update(StructManettes DataManettes)
    {
        m_State_Last = m_State;
        m_State = m_State_Next;   
     
#ifdef SerialDebug
//        Serial.println(m_Data.State);
#endif
        switch(m_State)
        {
            case ENUM_State_WaitStart : WaitStart(DataManettes) ; break;
            case ENUM_State_Start : UpdateGame(DataManettes) ; break;
            case ENUM_State_Fini : Fini(DataManettes); break; 
            case ENUM_State_Dead : Dead(DataManettes); break; 
        }
    }
    
    void WaitStart(StructManettes DataManettes)
    {      

          int positionBatX = m_Bat1.CUL.X ;
          int positionBat2X = m_Bat2.CUL.X ;
          
          if(Pl1Win == 0 && Pl2Win == 0)
          {
            if(DataManettes.DataManette1 & BTN_START) m_State_Next = ENUM_State_Start;
            if(DataManettes.DataManette1 & BTN_LEFT)--positionBatX;           
            else if(DataManettes.DataManette1 & BTN_RIGHT)++positionBatX ;
            if(DataManettes.DataManette2 & BTN_START) m_State_Next = ENUM_State_Start;
            if(DataManettes.DataManette2 & BTN_LEFT)--positionBat2X;
            else if(DataManettes.DataManette2 & BTN_RIGHT)++positionBat2X ;
          }
          
          else if(Pl1Win)
          {
            if(DataManettes.DataManette1 & BTN_START) m_State_Next = ENUM_State_Start;
            if(DataManettes.DataManette1 & BTN_LEFT)
            {
               --positionBatX;
               m_Ball.CUL.X=positionBatX;
                             
            
            }            
            else if(DataManettes.DataManette1 & BTN_RIGHT)
            {
              ++positionBatX ;
              m_Ball.CUL.X=positionBatX;
              
            }
            if(DataManettes.DataManette2 & BTN_LEFT)--positionBat2X;
            else if(DataManettes.DataManette2 & BTN_RIGHT)++positionBat2X ;
          }

          if(Pl2Win)
          {
            if(DataManettes.DataManette2 & BTN_START) m_State_Next = ENUM_State_Start;
            if(DataManettes.DataManette2 & BTN_LEFT)
            {
              --positionBat2X;
              m_Ball.CUL.X=positionBat2X;
              
            }
            else if(DataManettes.DataManette2 & BTN_RIGHT)
            {
              ++positionBat2X ;
              m_Ball.CUL.X=positionBat2X;
              
            }

            if(DataManettes.DataManette1 & BTN_LEFT) --positionBatX;
            else if(DataManettes.DataManette1 & BTN_RIGHT) ++positionBatX ;
          }



          positionBatX = max(1+(m_Bat1.Size.X+1)/2, min(29-m_Bat1.Size.X/2,positionBatX));//on borne au murs
          positionBat2X = max(1+(m_Bat2.Size.X+1)/2, min(29-m_Bat2.Size.X/2,positionBat2X));//on borne au murs
          m_Bat1.CUL.X  = positionBatX;
          m_Bat2.CUL.X  = positionBat2X;



        Draw();
        
       /* Text txt0("PRESS", m_Data.pDisplayManager);
        txt0.Draw({3,8}, C_WHITE); 
        Text txt1("START", m_Data.pDisplayManager);
        txt1.Draw({4,14}, C_WHITE); 
       // Text txt2("+", m_Data.pDisplayManager);
       // txt2.Draw({10,20}, C_WHITE); */
    }
    
    void Fini(StructManettes DataManettes)
    {      
        Draw();   
        if(m_State_Last != m_State) 
        { 
            m_nTimeState = millis();          
#ifdef SerialDebug
            Serial.println("-------------------------------");
            if( m_bWin ) Serial.println("---------- YOU WIN-------------");
            else Serial.println("----------GAME OVER----------");
            Serial.println("-------------------------------");
            Serial.print("nbr coup "); 
            Serial.println(m_nNbrCoup);  
#endif
        }
        else
        {          
            if(millis() - m_nTimeState > DELAY_STATE_FIN) 
            {
              m_State_Next = ENUM_State_WaitStart;
              //if( m_bWin )  nextLevel();
              //else 
              Init();
            }
            else
            {

                if( m_nLivePl1<1 )

                {
                    Text txt0("P2");
                    txt0.Draw({10,10}, C_RED, m_pDisplayManager);
                    Text txt1("WIN");
                    txt1.Draw({10,16}, C_RED, m_pDisplayManager);
                    
                }
                else 
                {
                    Text txt0("P1");
                    txt0.Draw({10,10}, C_BLUE, m_pDisplayManager);
                    Text txt1("WIN");
                    txt1.Draw({10,16}, C_BLUE, m_pDisplayManager);
                
                }

            }
        } 

                 Pl1Win=false;
                 Pl2Win=false;
    }
    void Dead(StructManettes DataManettes)
    {
        //Serial.println("entrée dns dead");
        if(m_nLivePl1 <= 0 || m_nLivePl2 <= 0) m_State_Next = ENUM_State_Fini;
        else
        {
            if(m_State_Last != m_State)m_nTimeState = millis();   
            if(millis() - m_nTimeState > DELAY_STATE_NEWLIVE) //on attent un temps minimum pour montrer au joueur qu'il a perdu
            {
                if(m_nLivePl1<m_nLivePl1Old)
                {
                  m_Ball.CUL = {14,1};
                  m_Ball.Vitesse.Y = +0.8;//-0.3; 
                  Pl1Win = false;
                  Pl2Win = true;

                }
                if(m_nLivePl2<m_nLivePl2Old)
                {
                  m_Ball.CUL = {14,28};
                  m_Ball.Vitesse.Y = -0.8;//-0.3;
                  Pl1Win = true;
                  Pl2Win = false;
                }
                //init la ball
                 
                m_Ball.Vitesse.X = 0.75;//0.3;
                
                
                //init la bat
                m_Bat1.Size = {6,1}; 
                m_Bat1.CUL = {14,29};
                m_Bat2.Size = {6,1}; 
                m_Bat2.CUL = {14,0};
                
                m_State_Next = ENUM_State_WaitStart;

                m_nLivePl1Old=m_nLivePl1;
                m_nLivePl2Old=m_nLivePl2;
            }
        }
        Draw();
    }
    void UpdateGame(StructManettes DataManettes)
    {      
        //update des positions
        //deplacement de la bat
        int positionBatX = m_Bat1.CUL.X ;
        int positionBat2X = m_Bat2.CUL.X ;
        if(DataManettes.DataManette1 & BTN_LEFT)            --positionBatX;
        else if(DataManettes.DataManette1 & BTN_RIGHT)      ++positionBatX ;
        if(DataManettes.DataManette2 & BTN_LEFT)            --positionBat2X;
        else if(DataManettes.DataManette2 & BTN_RIGHT)      ++positionBat2X ;
        positionBatX = max(1+(m_Bat1.Size.X+1)/2, min(29-m_Bat1.Size.X/2,positionBatX));//on borne au murs
        positionBat2X = max(1+(m_Bat2.Size.X+1)/2, min(29-m_Bat2.Size.X/2,positionBat2X));//on borne au murs
        m_Bat1.CUL.X  = positionBatX;
        m_Bat2.CUL.X  = positionBat2X;

        //nouvelle possiton de la ball
        float fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        float fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y;  
        fNextBallPositionX = max(0, min(29,fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29,fNextBallPositionY));//on borne au murs
       
        //verification si mur, 
        bool bFindMur = false;
        for(int ii = 0; ii < m_nMurSize && !bFindMur; ++ii)
        {
            //si murs
            if(m_Murs[ii].Center.X == (int)fNextBallPositionX && m_Murs[ii].Center.Y == (int)fNextBallPositionY)
            {
                bFindMur = true;
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Mur); 
                //recherche mode de rebond
                  bool Va(MurAt((int)fNextBallPositionX, (int)fNextBallPositionY+1)>-1);
                  bool Vb(MurAt((int)fNextBallPositionX, (int)fNextBallPositionY-1)>-1);
                  bool Ha(MurAt((int)fNextBallPositionX+1, (int)fNextBallPositionY)>-1);
                  bool Hb(MurAt((int)fNextBallPositionX-1, (int)fNextBallPositionY)>-1);
                  
                  //inversion de la vitesse en fonction 
                  if(Ha || Hb) 
                  {
                        m_Ball.Vitesse.Y = -m_Ball.Vitesse.Y;  
                        if((Va && m_Ball.Vitesse.Y>0) || (Va && m_Ball.Vitesse.Y>0)) 
                            m_Ball.Vitesse.X = -m_Ball.Vitesse.X;
                  }
                  else if(Va || Vb) m_Ball.Vitesse.X = -m_Ball.Vitesse.X;           
            }
        }
        //nouvelle possition recalculer
        fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y; 
        fNextBallPositionX = max(0, min(29,fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29,fNextBallPositionY));//on borne au murs
        
#ifdef SerialDebug
               // Serial.print("Briques restante : ");
               // Serial.println(m_Data.nNbrBrique_Actif);
#endif

        //nouvelle possition recalculer
        fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y; 
        fNextBallPositionX = max(0, min(29,fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29,fNextBallPositionY));//on borne au murs
        
        //verification si bat 
        if((int)fNextBallPositionY == (int)m_Bat1.CUL.Y)// vérification si la balle est sur la ligne de la bat (Y)
        {
            bool bFindBat1 = false;
            int nXImpactBat = 0;
            for(int ii = -(m_Bat1.Size.X+1)/2; ii < m_Bat1.Size.X/2 && !bFindBat1; ++ii)
            {
                if((int)(m_Bat1.CUL.X + ii) == (int)fNextBallPositionX) {bFindBat1 = true; nXImpactBat = ii;}                          
            }  
  
            if(bFindBat1) 
            {              
                //m_pAudio->SetVolume(25);  
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_PONG_Bat); 
                m_Ball.Vitesse.Y = -m_Ball.Vitesse.Y - CoefAccl;
                //m_Data.ball.Vitesse.X = m_Data.ball.Vitesse.X+((float)abs(nXImpactBat)/10.0f);
                //m_Ball.Vitesse.X = ((0.05f*nXImpactBat*nXImpactBat)+0.85f)*m_Ball.Vitesse.X; 
                m_Ball.Vitesse.X = m_Ball.Vitesse.X + CoefAccl; 
                Serial.print("Ball.VitesseX : ");
                Serial.println(m_Ball.Vitesse.X);
                Serial.print("Ball.VitesseY : ");
                Serial.println(m_Ball.Vitesse.Y);
                ++m_nNbrCoup;
#ifdef SerialDebug
                Serial.print("nbr coup "); 
                Serial.println(m_nNbrCoup);  
                Serial.print("position bat=" ); 
                Serial.println(nXImpactBat);  
                Serial.print("m_Data.ball.Vitesse.X= "); 
                Serial.println(m_Ball.Vitesse.X); 
#endif
            }
            else 
            {
                  m_State_Next = ENUM_State_Dead;
                  --m_nLivePl1;
                  m_bWin = false;              
            }
        }

/////////////////////////////////////////////////////
        //verification si bat2 
        else if((int)fNextBallPositionY == (int)m_Bat2.CUL.Y)// vérification si la balle est sur la ligne de la bat (Y)
        {
            bool bFindBat2 = false;
            int nXImpactBat2 = 0;
            for(int ii = -(m_Bat2.Size.X+1)/2; ii < m_Bat2.Size.X/2 && !bFindBat2; ++ii)
            {
                if((int)(m_Bat2.CUL.X + ii) == (int)fNextBallPositionX) {bFindBat2 = true; nXImpactBat2 = ii;}                          
            }  
  
            if(bFindBat2) 
            {              
                //m_pAudio->SetVolume(25);  
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_PONG_Bat); 
                m_Ball.Vitesse.Y = -m_Ball.Vitesse.Y + CoefAccl;
                //m_Data.ball.Vitesse.X = m_Data.ball.Vitesse.X+((float)abs(nXImpactBat)/10.0f);
                
                m_Ball.Vitesse.X = m_Ball.Vitesse.X - CoefAccl; 
                
                Serial.print("Ball.VitesseX : ");
                Serial.println(m_Ball.Vitesse.X);
                Serial.print("Ball.VitesseY : ");
                Serial.println(m_Ball.Vitesse.Y);
                //m_Ball.Vitesse.X = ((0.05f*nXImpactBat2*nXImpactBat2)+0.85f)*m_Ball.Vitesse.X;
  
                ++m_nNbrCoup;
#ifdef SerialDebug
                Serial.print("nbr coup "); 
                Serial.println(m_nNbrCoup);  
                Serial.print("position bat=" ); 
                Serial.println(nXImpactBat2);  
                Serial.print("m_Data.ball.Vitesse.X= "); 
                Serial.println(m_Ball.Vitesse.X); 
#endif
            }
            else 
            {
                  m_State_Next = ENUM_State_Dead;
                  --m_nLivePl2;
                  m_bWin = false;              
            }
        }
        //////////////////////////////////////
        
        //nouvelle possition recalculer
        fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y; 
        fNextBallPositionX = max(0, min(29, fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29, fNextBallPositionY));//on borne au murs
                
        m_Ball.CUL.X = fNextBallPositionX;
        m_Ball.CUL.Y = fNextBallPositionY;
        
        Draw();
    }    
    
    private:
    DisplayManager* m_pDisplayManager;  //Poiteur sur le tableau d'affichage (table led) (forme matricielle)
    MyDFPlayer* m_pAudio; 
        
    Bat m_Bat1, m_Bat2;
    Ball m_Ball;
    
    int m_nMurSize; //toute les briques on la meme taille
    Mur* m_Murs;

    //infos sur la partie en cours
    unsigned int m_nNbrCoup;
    Game_State m_State, m_State_Last, m_State_Next;
    unsigned long m_nTimeState; 
    bool m_bWin;
    int m_nLivePl1;
    int m_nLivePl2;
    int m_nLivePl1Old;
    int m_nLivePl2Old;
    int m_nLevel; 
    bool Pl1Win = false;
    bool Pl2Win = false;
    float fNextBallPositionX = 0;
    float CoefAccl=0.01;

    int MurAt(unsigned int Xcoor, unsigned int Ycoor)
    {
        for(int ii = 0; ii < m_nMurSize; ++ii)
            if(m_Murs[ii].Center.X == Xcoor && m_Murs[ii].Center.Y == Ycoor)
                return ii;
       return -1;
    } 

    void ResetMur(void)
    {
        m_nMurSize = 0; 
        if(m_Murs != NULL)     { delete[] m_Murs; m_Murs = NULL;}             
    }
    
    void AddMur(Mur newMur)
    { 
        Mur* Table = new Mur[m_nMurSize + 1];
        for(unsigned int ii = 0; ii < m_nMurSize; ++ii)
        {
            Table[ii] = m_Murs[ii];
        }
        delete[] m_Murs;
        m_Murs = Table;
        
        m_Murs[m_nMurSize] = newMur;
        ++m_nMurSize;
    }
};

#endif

