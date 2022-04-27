#ifndef _GAME_CASSEBRIQUE_H_
#define _GAME_CASSEBRIQUE_H_

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
#define CASSEBRIQUE_PATH_LVL ("Game/CasseB/lvl_")//suivi du numereau de niveau et de l'extention '.txt'
                    
class Game_CasseBrique : public Game
{
    public:    
    Game_CasseBrique(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
    {
        //recuperation du display manager
        m_pDisplayManager = pDisplayManager; 
        m_pAudio = pAudio; 
        
        //reset de mur
        m_nMurSize = 0;
        m_Murs = NULL;
        
        //reset de brique
        m_nBriqueSize = 0;
        m_nNbrBrique_Actif = 0;
        m_Briques = NULL;
                
        //reset de la ball
        m_Ball.Size = {0,0};
        m_Ball.Vitesse = {0,0};
        m_Ball.CUL = {0,0};
        m_Ball.Color = C_YELLOW;
        
        //reset de la bat
        m_Bat.Size = {0,0};
        m_Bat.CUL = {0,0};
        m_Bat.Color = C_BLUE;

        //game
        m_State = ENUM_State_WaitStart;
        m_State_Last = m_State ;
        m_State_Next = m_State ;
        m_nTimeState = 0;
        m_bWin = false;

        m_nLive = 0;    
        m_nLevel = 0;
    }
    
    void Load(unsigned int nLvl)
    {
        ResetBrique();
        ResetMur();
        
        String strPath = CASSEBRIQUE_PATH_LVL;
        strPath = strPath + nLvl;
        strPath = strPath + ".txt";

        if (SD.exists(strPath) )
        {
            File myFile = SD.open(strPath, FILE_READ);
            
            //lecture type
            int Type = 0;
            ColorRGB color = {0,0,0}; 
            while(myFile.available())
            {
                Type = myFile.parseInt(); 
                switch(Type)
                {
                    case 0 ://Mur   
                        Mur newMur;
                        myFile.read();
                        newMur.Center.X = myFile.parseInt();myFile.read();
                        newMur.Center.Y = myFile.parseInt();myFile.read();
                        color.R = myFile.parseInt();myFile.read();
                        color.G = myFile.parseInt();myFile.read();
                        color.B = myFile.parseInt();myFile.read();
                        newMur.Color = Adafruit_NeoPixel::Color(color.R, color.G, color.B);
                        AddMur(newMur);
                        break;
                    case 1 ://Brique    
                        Brique newBrique;
                        myFile.read();
                        newBrique.Actif = true;
                        newBrique.Size.X = myFile.parseInt();myFile.read();
                        newBrique.Size.Y = myFile.parseInt();myFile.read();
                        newBrique.CUL.X = myFile.parseInt();myFile.read();
                        newBrique.CUL.Y = myFile.parseInt();myFile.read();
                        color.R = myFile.parseInt();myFile.read();
                        color.G = myFile.parseInt();myFile.read();
                        color.B = myFile.parseInt();myFile.read();
                        newBrique.Color = Adafruit_NeoPixel::Color(color.R, color.G, color.B);
                        AddBrique(newBrique);
                        break;
                    case 2 ://Ball
                        myFile.read();
                        m_Ball.Size.X = myFile.parseInt();myFile.read();
                        m_Ball.Size.Y = myFile.parseInt();myFile.read();
                        m_Ball.Vitesse.X = myFile.parseFloat();myFile.read();
                        m_Ball.Vitesse.Y = myFile.parseFloat();myFile.read();
                        m_Ball.CUL.X = myFile.parseInt();myFile.read();
                        m_Ball.CUL.Y = myFile.parseInt();myFile.read();
                        color.R = myFile.parseInt();myFile.read();
                        color.G = myFile.parseInt();myFile.read();
                        color.B = myFile.parseInt();myFile.read();
                        break;
                    case 3 ://Batt
                        myFile.read();
                        m_Bat.Size.X = myFile.parseInt();myFile.read();
                        m_Bat.Size.Y = myFile.parseInt();myFile.read();
                        m_Bat.CUL.X = myFile.parseInt();myFile.read();
                        m_Bat.CUL.Y = myFile.parseInt();myFile.read();
                        color.R = myFile.parseInt();myFile.read();
                        color.G = myFile.parseInt();myFile.read();
                        color.B = myFile.parseInt();myFile.read();
                        m_Bat.Color  = Adafruit_NeoPixel::Color(color.R, color.G, color.B);
                        break;
                    case 4 ://Live
                        myFile.read();
                        color.R = myFile.parseInt();myFile.read();
                        color.G = myFile.parseInt();myFile.read();
                        color.B = myFile.parseInt();myFile.read();
                        m_ColorLivePl1 = Adafruit_NeoPixel::Color(color.R, color.G, color.B);
                        break;
                }
            }            
            myFile.close();
        }
    }
    
    void Init(void)
    {         
        Load(0);             

        //init info partie
        m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Start);
        m_nNbrCoup = 0;
        m_State = ENUM_State_WaitStart;
        m_State_Last = m_State ;
        m_nTimeState = 0;
        m_bWin = false;
        m_nLive = 3;        
    }
    
    void nextLevel(void)
    {
        Load(++m_nLevel);   
        
        //init info partie
        m_nNbrCoup = 0;
        m_State = ENUM_State_WaitStart;
        m_State_Last = m_State ;
        m_nTimeState = 0;
        m_bWin = false;
        ++m_nLive;             
    }
    
    void Draw(void)
    {
        m_pDisplayManager->OFF();
        //on donne au displaymanager les type pour chaque pixels        
        //on donne les murs      
        for(int ii = 0; ii < m_nMurSize; ++ii)//on onne les murs
        {
            m_pDisplayManager->SetPixel(m_Murs[ii].Center, m_Murs[ii].Color );
        }
            
        //on donne les briques
        for(int ii = 0; ii < m_nBriqueSize; ++ii)//on donne les briques
            if(m_Briques[ii].Actif)
                for(int xx = 0; xx < m_Briques[ii].Size.X; ++xx)
                    for(int yy = 0; yy < m_Briques[ii].Size.Y; ++yy)
                    {
                        m_pDisplayManager->SetPixel(m_Briques[ii].CUL.X+xx, m_Briques[ii].CUL.Y+yy, m_Briques[ii].Color );
                    }
        
        //on donne la balle   
        for(int xx = 0; xx < m_Ball.Size.X; ++xx)
            for(int yy = 0; yy < m_Ball.Size.Y; ++yy)
            {
              m_pDisplayManager->SetPixel(m_Ball.CUL.X+xx, m_Ball.CUL.Y+yy , m_Ball.Color );
            }
        
        //on donne la bat        
        for(int ii = -(m_Bat.Size.X+1)/2; ii < m_Bat.Size.X/2; ++ii)
        {
            m_pDisplayManager->SetPixel(m_Bat.CUL.X + ii, m_Bat.CUL.Y, m_Bat.Color );
        }    

        //on donne les vies
        for(int ii = 0; ii < m_nLive; ++ii)
        {
          m_pDisplayManager->SetPixel(29, 29-ii, m_ColorLivePl1);
        }
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
            case ENUM_State_Dead : Dead(DataManettes); break; 
        }
    }
    
    void WaitStart(StructManettes DataManettes)
    {      
        if(DataManettes.DataManette1 & BTN_START) m_State_Next = ENUM_State_Start;
        else if(DataManettes.DataManette1 & BTN_LEFT)
        {      
            --m_Ball.CUL.X; 
            --m_Bat.CUL.X; 
        }
        else if (DataManettes.DataManette1 & BTN_RIGHT)   
        {
            ++m_Ball.CUL.X; 
            ++m_Bat.CUL.X; 
        }
        else if(m_State_Last != m_State) 
        {       
#ifdef SerialDebug
            Serial.println("Press 'Start' for play game"); 
#endif           
        }
        
        m_Bat.CUL.X  = max(1+(m_Bat.Size.X+1)/2, min(29-m_Bat.Size.X/2,m_Bat.CUL.X ));//on borne au murs
        m_Ball.CUL.X  = max(1+(m_Bat.Size.X+1)/2, min(29-m_Bat.Size.X/2,m_Ball.CUL.X));//on borne au murs
        
        Draw();
        
        Text txt0("PRESS");
        txt0.Draw({3,8}, C_WHITE, m_pDisplayManager); 
        Text txt1("START");
        txt1.Draw({4,14}, C_WHITE, m_pDisplayManager); 
        Text txt2("+");
        txt2.Draw({10,20}, C_WHITE, m_pDisplayManager); 
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
              if( m_bWin )  nextLevel();
              else Init();
            }
            else
            {
                if( m_bWin ) 
                {
                    g_Texts[0].Draw({10,10}, C_WHITE, m_pDisplayManager); //"YOU"
                    g_Texts[1].Draw({10,16}, C_WHITE, m_pDisplayManager); //"WIN"
                }
                else 
                {
                    g_Texts[0].Draw({10,10}, C_WHITE, m_pDisplayManager); //"YOU"
                    g_Texts[2].Draw({10,16}, C_WHITE, m_pDisplayManager); //"LOSE"                    
                }
            }
        }        
    }
    
    void Dead(StructManettes DataManettes)
    {        
        if(m_nLive <= 0) 
        {                  
            m_pAudio->SetVolume(25);  
            m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Dead); 
            m_State_Next = ENUM_State_Fini;
        }
        else
        {
            if(m_State_Last != m_State)m_nTimeState = millis();   
            if(millis() - m_nTimeState > DELAY_STATE_NEWLIVE) //on attent un temps minimum pour montrer au joueur qu'il a perdu
            {
                //init la ball au cenrte
                m_Ball.CUL = {14,28}; 
                m_Ball.Vitesse.X = 0.75;//0.3;
                m_Ball.Vitesse.Y = -0.8;//-0.3;
                
                //init la bat au centre
                m_Bat.Size = {6,1}; 
                m_Bat.CUL = {14,29};
                
                m_State_Next = ENUM_State_WaitStart;
            }
        }
        Draw();
    }
    
    void UpdateGame(StructManettes DataManettes)
    {      
        //update des positions
        
        //deplacement de la bat
        int positionBatX = m_Bat.CUL.X ;
        if(DataManettes.DataManette1 & BTN_LEFT)       --positionBatX;
        if(DataManettes.DataManette1 & BTN_RIGHT)      ++positionBatX ;
        positionBatX = max(1+(m_Bat.Size.X+1)/2, min(29-m_Bat.Size.X/2,positionBatX));//on borne au murs
        m_Bat.CUL.X  = positionBatX;

        //nouvelle possiton de la ball
        float fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        float fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y;  
        fNextBallPositionX = max(0, min(29,fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29,fNextBallPositionY));//on borne au murs
       
        //verification si mur, 
        bool bFindMur = false;
        bool Va, Vb, Ha, Hb;
        for(int ii = 0; ii < m_nMurSize && !bFindMur; ++ii)
        {
            //si murs
            if(m_Murs[ii].Center.X == (int)fNextBallPositionX && m_Murs[ii].Center.Y == (int)fNextBallPositionY)
            {
                bFindMur = true;
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Mur);
                //recherche mode de rebond
                  Va=(MurAt((int)fNextBallPositionX, (int)fNextBallPositionY+1)>-1);
                  Vb=(MurAt((int)fNextBallPositionX, (int)fNextBallPositionY-1)>-1);
                  Ha=(MurAt((int)fNextBallPositionX+1, (int)fNextBallPositionY)>-1);
                  Hb=(MurAt((int)fNextBallPositionX-1, (int)fNextBallPositionY)>-1);
                  
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
        
        //verification si brique sur le chemain de la balle       
        bool bFindBrique = false;
        for(int ii = 0; ii < m_nBriqueSize && !bFindBrique; ++ii)
        {
            //si la balle rencontre une brique actif sur sont chemain
            int nBrique = BriqueAt((int)fNextBallPositionX, (int)fNextBallPositionY);
            if(nBrique > -1)
            {
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Brique);
                bFindBrique = true;
                m_Briques[nBrique].Actif = false;
                --m_nNbrBrique_Actif; 
                
#ifdef SerialDebug
                Serial.print("Briques restante : ");
                Serial.println(m_nNbrBrique_Actif);
#endif

                if(m_nNbrBrique_Actif<1)
                {
                    m_State_Next = ENUM_State_Fini;
                    m_bWin = true;               
                }
                else
                {
                    //recherche mode de rebond
                    Va=(BriqueAt((int)fNextBallPositionX, (int)fNextBallPositionY+1)>-1);
                    Vb=(BriqueAt((int)fNextBallPositionX, (int)fNextBallPositionY-1)>-1);
                    Ha=(BriqueAt((int)fNextBallPositionX+1, (int)fNextBallPositionY)>-1);
                    Hb=(BriqueAt((int)fNextBallPositionX-1, (int)fNextBallPositionY)>-1);
                    
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
        }
        //nouvelle possition recalculer
        fNextBallPositionX = m_Ball.CUL.X + m_Ball.Vitesse.X;
        fNextBallPositionY = m_Ball.CUL.Y + m_Ball.Vitesse.Y; 
        fNextBallPositionX = max(0, min(29,fNextBallPositionX));//on borne au murs
        fNextBallPositionY = max(0, min(29,fNextBallPositionY));//on borne au murs
        
        //verification si bat 
        if((int)fNextBallPositionY == (int)m_Bat.CUL.Y)// vérification si la balle est sur la ligne de la bat (Y)
        {
            bool bFindBat = false;
            int nXImpactBat = 0;
            for(int ii = -(m_Bat.Size.X+1)/2; ii < m_Bat.Size.X/2 && !bFindBat; ++ii)
            {
                if((int)(m_Bat.CUL.X + ii) == (int)fNextBallPositionX) {bFindBat = true; nXImpactBat = ii;}                          
            }  
  
            if(bFindBat) 
            {
                //m_pAudio->SetVolume(25);  
                m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Bat); 
                m_Ball.Vitesse.Y = -m_Ball.Vitesse.Y;
                //m_Data.ball.Vitesse.X = m_Data.ball.Vitesse.X+((float)abs(nXImpactBat)/10.0f);
                m_Ball.Vitesse.X = ((0.05f*nXImpactBat*nXImpactBat)+0.85f)*m_Ball.Vitesse.X;
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
                  m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Dead);
                  m_State_Next = ENUM_State_Dead;
                  --m_nLive;
                  m_bWin = false;              
            }
        }
        
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
    //objet Bat
    Bat m_Bat;
    //objet Ball
    Ball m_Ball;
    
    //Tableau des Murs
    int m_nMurSize; 
    Mur* m_Murs;

    //Tableau des briques
    int m_nBriqueSize;
    int m_nNbrBrique_Actif;
    Brique* m_Briques;

    //infos sur la partie en cours
    unsigned int m_nNbrCoup;
    Game_State m_State, m_State_Last, m_State_Next;
    unsigned long m_nTimeState; 
    bool m_bWin;
    int m_nLive;
    int m_nLevel; 
      
   /********************************************************************/ 
    int BriqueAt(unsigned int Xcoor, unsigned int Ycoor)
    {
        int nSizeX(0), nSizeY(0);
        int CulX(0), CulY(0);
        for(int ii = 0; ii < m_nBriqueSize; ++ii)
        {
            if(m_Briques[ii].Actif)
            {
                nSizeX = m_Briques[ii].Size.X;
                nSizeY = m_Briques[ii].Size.Y;
                CulX = m_Briques[ii].CUL.X;
                CulY = m_Briques[ii].CUL.Y;
                for(int xx = 0; xx < nSizeX; ++xx)
                    for(int yy = 0; yy < nSizeY; ++yy)
                        if(CulX+xx == Xcoor && CulY+yy == Ycoor)
                            return ii;
            }
        }
       return -1;
    } 
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
    void ResetBrique(void)
    {        
        m_nBriqueSize = 0;
        m_nNbrBrique_Actif = 0;
        if(m_Briques != NULL)  {delete[] m_Briques; m_Briques = NULL;}
    }
    void AddBrique(Brique newBrique)
    {
        Brique* Table = new Brique[m_nBriqueSize + 1];
        for(unsigned int ii = 0; ii < m_nBriqueSize; ++ii)
        {
            Table[ii] = m_Briques[ii];
        }
        delete[] m_Briques;
        m_Briques = Table;
        
        m_Briques[m_nBriqueSize] = newBrique; 
        ++m_nBriqueSize;
        ++m_nNbrBrique_Actif;       
    }
};

#endif
