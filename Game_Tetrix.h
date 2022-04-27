#ifndef _GAME_TETRIX_H_
#define _GAME_TETRIX_H_

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

struct GameTetrix_Shape
{
    char Pixel_Color;
    int nRotSize;
    Vect2D** Rot;    
};

#define Game_Tetrix_HorizontalMoveTime (50)
#define Game_Tetrix_VerticalMoveTime (1000)
#define Game_Tetrix_RotationTime (50)
class Game_Tetrix : public Game 
{
    public:    
    Game_Tetrix(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
    {
        //recuperation du display manager
        m_pDisplayManager = pDisplayManager; 
        m_pAudio = pAudio;       
        
        m_nScore = 0; 
        m_nLevel = 0;    
        m_PixelBriques = new char*[10];
        for(int ii = 0; ii < 10; ++ii) 
        {
            m_PixelBriques[ii] = new char[29];
            for(int jj = 0; jj < 29; ++jj)
                m_PixelBriques[ii][jj] = 0;
        }       
         
        Tetrix_Shapes_Size = 7;        
        Tetrix_Shapes = new GameTetrix_Shape[7]
        {
            //cube
            GameTetrix_Shape{1, 1, new Vect2D*[1]{new Vect2D[4]{{0,0}, {1,0}, {0,1}, {1,1}}}},
            //Bare
            GameTetrix_Shape{2, 2, new Vect2D*[2]{new Vect2D[4]{{0,0}, {0,1}, {0,2}, {0,3}}, new Vect2D[4]{{0,0}, {1,0}, {2,0}, {3,0}}}},
            //Z     
            GameTetrix_Shape{3, 2, new Vect2D*[2]{new Vect2D[4]{{0,0}, {0,1}, {1,1}, {1,2}}, new Vect2D[4]{{0,1}, {1,1}, {1,0}, {2,0}}}},
            //Zinv
            GameTetrix_Shape{4, 2, new Vect2D*[2]{new Vect2D[4]{{1,0}, {1,1}, {0,1}, {0,2}}, new Vect2D[4]{{0,0}, {1,0}, {1,1}, {2,1}}}},
            //piramide
            GameTetrix_Shape{5, 4, new Vect2D*[4]{new Vect2D[4]{{0,0}, {1,0}, {2,0}, {1,1}}, new Vect2D[4]{{0,0}, {0,1}, {0,2}, {1,1}}, new Vect2D[4]{{0,1}, {1,1}, {2,1}, {1,0}}, new Vect2D[4]{{0,1}, {1,0}, {1,1}, {1,2}}}},
            //L
            GameTetrix_Shape{6, 4, new Vect2D*[4]{new Vect2D[4]{{0,0}, {0,1}, {0,2}, {1,0}}, new Vect2D[4]{{0,0}, {1,0}, {2,0}, {2,1}}, new Vect2D[4]{{0,2}, {1,2}, {1,1}, {1,0}}, new Vect2D[4]{{0,0}, {0,1}, {1,1}, {2,1}}}},
            //Linv
            GameTetrix_Shape{7, 4, new Vect2D*[4]{new Vect2D[4]{{0,0}, {1,0}, {1,1}, {1,2}}, new Vect2D[4]{{0,1}, {1,1}, {2,1}, {2,0}}, new Vect2D[4]{{1,2}, {0,2}, {0,1}, {0,0}}, new Vect2D[4]{{0,1}, {0,0}, {1,0}, {2,0}}}}
        };   
        Tetrix_Color = new ColorRGB[8]
        {
          {0,0,0},
          {250,0,0},
          {0,250,0},
          {0,0,250},
          {100,100,50},
          {100,50,100},
          {50,100,100},
          {100,100,100},
        };   

        NextBrique = Tetrix_Shapes[random(0,7)];        
        NextBrique_IndiceRot = random(0,NextBrique.nRotSize);
        NewShape();
        
        m_State = ENUM_State_WaitStart;
        m_State_Next = ENUM_State_WaitStart;

        m_Time_MoveHorizontal = millis();
        m_Time_MoveVertical = m_Time_MoveHorizontal; 
        m_Time_Rotation = m_Time_MoveHorizontal;
    }
    
    void Load(unsigned int nLvl)
    {
        //TODO
    }    
    
    void Init(void)
    {         
        Load(0); 
        m_State = m_State_Next;
        m_State_Next = ENUM_State_WaitStart;
    }
    
    void nextLevel(void)
    {    
        //TODO
    }
    
    void Draw(void)
    {
        m_pDisplayManager->OFF();

        //on desine le mur exterieur
        for(int yy = 0; yy < 30; ++yy)
        {
            m_pDisplayManager->SetPixel(0, yy, C_WHITE );
            m_pDisplayManager->SetPixel(11, yy, C_WHITE );
        }
        for(int ii = 0; ii < 6; ++ii)
        {
            m_pDisplayManager->SetPixel(12+ii, 6, C_WHITE);
            m_pDisplayManager->SetPixel(18, ii, C_WHITE);
        }
        for(int ii = 0; ii < 10; ++ii)
        {
            m_pDisplayManager->SetPixel(1+ii, 29, C_WHITE);
        }
        
        //on desine les brique deja tombees
        for(int xx = 0; xx < 10; ++xx)
        {
            for(int yy = 0; yy < 29; ++yy)
            {
                m_pDisplayManager->SetPixel(xx+1, yy, Tetrix_Color[m_PixelBriques[xx][yy]]);
            }
        }
        
        //on desine la brique qui tombe      
        Vect2D coor_={0,0};     
        ColorRGB colorBirque_Current = Tetrix_Color[CurrentBrique.Pixel_Color];
        for(int ii = 0; ii < 4; ++ii)
        {
            Vect2D* pBrique =  &CurrentBrique.Rot[CurrentBrique_IndiceRot][ii]; 
            coor_.X = pBrique->X + fCurrentBrique_Coordonnees.X;
            coor_.Y = pBrique->Y + fCurrentBrique_Coordonnees.Y;
               
            if(coor_.Y >= 0 && coor_.Y<30 && coor_.X>=0 && coor_.X<10 )
            {
                m_pDisplayManager->SetPixel(coor_.X+1, coor_.Y, colorBirque_Current);
            }
        }
        
        //on dessine la prochaine brique
        ColorRGB colorBirque_Next = Tetrix_Color[NextBrique.Pixel_Color];
        for(int ii = 0; ii < 4; ++ii)
        {
            Vect2D* pBrique =  &NextBrique.Rot[NextBrique_IndiceRot][ii]; 
            m_pDisplayManager->SetPixel(pBrique->X+13, pBrique->Y+1, colorBirque_Next);
        } 

        //on ecrit le score
        Text txt0(m_nScore);
        txt0.Draw({13,9}, C_WHITE, m_pDisplayManager);
        //on ecrit le Level
        Text txt1(m_nLevel);
        txt1.Draw({13,15}, C_WHITE, m_pDisplayManager);
    }
    
    void Update(StructManettes DataManettes)
    {    
        m_State_Last = m_State;
        m_State = m_State_Next;  
        switch(m_State)
        {
            case ENUM_State_WaitStart : WaitStart(DataManettes) ;break;
            case ENUM_State_Start : UpdateGame(DataManettes) ;break;
            case ENUM_State_Fini : Fini(DataManettes);break; 
            case ENUM_State_Dead : Dead(DataManettes);break; 
        }        
    }

    void WaitStart(StructManettes DataManettes)
    {      
        m_pDisplayManager->OFF();
        if(DataManettes.DataManette1 & BTN_START) 
        {
            m_State_Next = ENUM_State_Start;
            m_pAudio->SetVolume(15);  
            m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_TETRIS_Music); 
        }
        else if(m_State_Last != m_State) 
        {
            Serial.println("Press 'Start' for play game");            
        }
        
        Text txt0("PRESS");
        txt0.Draw({3,8}, C_WHITE, m_pDisplayManager); 
        Text txt1("START");
        txt1.Draw({4,14}, C_WHITE, m_pDisplayManager); 
    }
    
    void UpdateGame(StructManettes DataManettes)
    {             
        //mouve/rotation en fonction de la manette 
        unsigned long Time = millis();
        if(Time - m_Time_MoveHorizontal >= Game_Tetrix_HorizontalMoveTime)
        {
            if(DataManettes.DataManette1 & BTN_LEFT)      
            { 
                m_Time_MoveHorizontal = Time;
                bool bEnd = false;
                for(int ii = 0; ii < 4; ++ii)
                {
                        
                    Vect2D coor = CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
                    coor.X += fCurrentBrique_Coordonnees.X-1;
                    coor.Y += fCurrentBrique_Coordonnees.Y;

                    coor.X = max(coor.X, 0);
                    if( m_PixelBriques[coor.X][coor.Y] != 0) bEnd = true;
                }
                
                if(!bEnd) --fCurrentBrique_Coordonnees.X; 
            }
            else if(DataManettes.DataManette1 & BTN_RIGHT)
            {
                m_Time_MoveHorizontal = Time;
                
                bool bEnd = false;
                for(int ii = 0; ii < 4; ++ii)
                {      
                    Vect2D coor = CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
                    coor.X += fCurrentBrique_Coordonnees.X+1;
                    coor.Y += fCurrentBrique_Coordonnees.Y;
                    
                    coor.X = min(coor.X, 9);
                    if( m_PixelBriques[coor.X][coor.Y] != 0) bEnd = true;
                }
                
                if(!bEnd) ++fCurrentBrique_Coordonnees.X; 
                
            }
        }
        
        if(Time - m_Time_Rotation >= Game_Tetrix_RotationTime && DataManettes.DataManette1 & BTN_A)
        {          
            int nextIndiceRotation = (CurrentBrique_IndiceRot+1)%CurrentBrique.nRotSize;
            
            bool bEnd = false;
            for(int ii = 0; ii < 4; ++ii)
            {                        
                Vect2D coor = CurrentBrique.Rot[nextIndiceRotation][ii];
                coor.X += fCurrentBrique_Coordonnees.X;
                coor.Y += fCurrentBrique_Coordonnees.Y;

                coor.X = max(min(coor.X,30), 0);
                coor.Y = max(min(coor.Y,30), 0);
                if( m_PixelBriques[coor.X][coor.Y] != 0) bEnd = true;
            }
            
            if(!bEnd)   
            {
                CurrentBrique_IndiceRot = nextIndiceRotation;
                int tempo_ = nCurrentBrique_Size.X;
                nCurrentBrique_Size.X = nCurrentBrique_Size.Y;
                nCurrentBrique_Size.Y = tempo_;
            }
            
            m_Time_Rotation = Time;
        }
        fCurrentBrique_Coordonnees.X = min(9-nCurrentBrique_Size.X, max(0, fCurrentBrique_Coordonnees.X));   

        if(DataManettes.DataManette1 & BTN_DOWN)  
        {
            m_Time_MoveVertical -= 300;
        }    
        if(DataManettes.DataManette1 & BTN_B)  
        {
            bool bEnd = false; 
            for(int ii = 0; ii < 30 && !bEnd ; ++ii)
            {         
                bEnd = false;  
                for(int ii = 0; ii < 4; ++ii)
                {
                    Vect2D coor = CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
                    coor.X += fCurrentBrique_Coordonnees.X;
                    coor.Y += fCurrentBrique_Coordonnees.Y + 1;
                    
                    if( coor.Y >= 29 || m_PixelBriques[coor.X][coor.Y] != 0) bEnd = true;
                }
                if(!bEnd) //on fait decendre l'objet si rien en dessous
                {
                    ++fCurrentBrique_Coordonnees.Y;              
                }    
            }
        }    
        
        if(Time - m_Time_MoveVertical >= Game_Tetrix_VerticalMoveTime*(1.0f/(m_nScore+1.0f)))
        {
            m_Time_MoveVertical = Time;
            //decendre la forme
            //recherche min de la forme
            bool bEnd = false;
            
            for(int ii = 0; ii < 4; ++ii)
            {
                Vect2D coor = CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
                coor.X += fCurrentBrique_Coordonnees.X;
                coor.Y += fCurrentBrique_Coordonnees.Y + 1;
                
                if( coor.Y >= 29 || m_PixelBriques[coor.X][coor.Y] != 0) bEnd = true;
            }
            if(!bEnd) //on fait decendre l'objet si rien en dessous
            {
                ++fCurrentBrique_Coordonnees.Y;              
            }        
            else //sinon on tranfert la forme dans la table des pixel, on genere la forme suivante, et on fixe l'objet dans le plateau et on vide l'objet a decendre.
            {                       
                Vect2D** pShape =  &CurrentBrique.Rot[CurrentBrique_IndiceRot];
                for(int ii = 0; ii < 4; ++ii)
                {   
                    Vect2D coor_= CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
                    coor_.X += fCurrentBrique_Coordonnees.X;
                    coor_.Y += fCurrentBrique_Coordonnees.Y;
                   
                    if(coor_.Y >= 0 && coor_.Y<29 && coor_.X>=0 && coor_.X<10 )
                    {
                        m_PixelBriques[coor_.X][coor_.Y] = CurrentBrique.Pixel_Color;
                    }   
                    else
                    {
                        m_State_Next = ENUM_State_Dead;
                    }
                }
                
                if( m_State != ENUM_State_Dead)
                {                 
                    //test lignes completes si oui on supprime les lignes pleines et on descend les autres
                    for(int yy = 0; yy < 29; ++yy)
                    {  
                      //calcul briques sur la lignes
                        int nNbrInLine = 0;
                        for(int xx = 0; xx < 10; ++xx)
                        {  
                            if(m_PixelBriques[xx][yy] != 0)  
                                ++nNbrInLine;
                        }
                        //si ligne plein alors on a 10 birques
                        if(nNbrInLine == 10)
                        {
                            //on met la ligne a vide
                            for(int xx = 0; xx < 10; ++xx)
                                m_PixelBriques[xx][yy]  = 0;       
                                 
                            for(int yy_ = yy; yy_ > 0; --yy_)
                            {                              
                                for(int xx = 0; xx < 10; ++xx)
                                    m_PixelBriques[xx][yy_]  = m_PixelBriques[xx][yy_-1] ;       
                            }
                            for(int xx = 0; xx < 10; ++xx)
                                m_PixelBriques[xx][0]  = 0; 
                                
                            ++m_nScore;
                            m_nLevel = min(m_nScore/3,99);
                        }                        
                    }
    
                    //nouvelle forme 
                    NewShape(); 
                }   
            }         
        }
        
        Draw();
    }
    
    void Fini(StructManettes DataManettes)
    {             
        Serial.println("STATE FINI");       
        if(m_State_Last != m_State) 
        { 
            m_nTimeState = millis(); 
        }
        else
        {
            if(millis() - m_nTimeState > DELAY_STATE_FIN) 
            {
                Text txt0("YOUR");
                txt0.Draw({8,10}, C_GREEN, m_pDisplayManager); 
                Text txt1("SCORE");
                txt1.Draw({5,16}, C_GREEN, m_pDisplayManager); 
                Text txt2("XXXX");
                txt2.Draw({5,22}, C_GREEN, m_pDisplayManager); 
            }
            else
            {
                m_State_Next = ENUM_State_WaitStart;
            }
        }
    }
    
    void Dead(StructManettes DataManettes)
    {                 
        Serial.println("STATE DEAD");
        m_pAudio->SetVolume(25);  
        m_pAudio->Play(AUDIO_SELECTION::eAudion_Selection_CASSEBRIQUE_Dead); 
        m_State_Next = ENUM_State_Fini;
     
        Draw();
    }

    void NewShape(void)
    {
        CurrentBrique = NextBrique;
        CurrentBrique_IndiceRot = NextBrique_IndiceRot;
        
        NextBrique = Tetrix_Shapes[random(0,7)];
        NextBrique_IndiceRot = random(0,NextBrique.nRotSize);

        nCurrentBrique_Size = {0,0};        
        for(int ii = 0; ii < 4; ++ii)
        {
            Vect2D* pBrique =  &CurrentBrique.Rot[CurrentBrique_IndiceRot][ii];
            if(nCurrentBrique_Size.X < pBrique->X)  nCurrentBrique_Size.X = pBrique->X;
            if(nCurrentBrique_Size.Y < pBrique->Y)  nCurrentBrique_Size.Y = pBrique->Y;
        }             
        
        fCurrentBrique_Coordonnees = {5,-(nCurrentBrique_Size.Y+1) };
    }
    
    private:    
    DisplayManager* m_pDisplayManager;  //Poiteur sur le tableau d'affichage (table led) (forme matricielle)
    MyDFPlayer* m_pAudio; 
    
    int m_nScore; // nombre de lignes
    int m_nLevel;
    unsigned long m_nTimeState; 

    char** m_PixelBriques;

    //table des forme
    int Tetrix_Shapes_Size;    
    GameTetrix_Shape* Tetrix_Shapes;
    ColorRGB* Tetrix_Color;

    //forme en coure de chute
    GameTetrix_Shape CurrentBrique;
    int CurrentBrique_IndiceRot;
    Vect2D_f fCurrentBrique_Coordonnees;
    Vect2D nCurrentBrique_Size;
    
    //forme qui chutera apres la current
    GameTetrix_Shape NextBrique;
    int NextBrique_IndiceRot;

    //time
    unsigned long m_Time_MoveHorizontal, m_Time_MoveVertical, m_Time_Rotation;
};

#endif

