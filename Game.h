#ifndef _GAME_H_
#define _GAME_H_

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

//Game States
enum Game_State
{  
    ENUM_State_WaitStart,
    ENUM_State_Start,
    ENUM_State_Fini,
    ENUM_State_Dead        
};

//Game Objects
struct Brique
{
    bool Actif;
    Vect2D CUL;       //Corner Uper Left
    Vect2D Size;      
    uint32_t Color;
};

struct Ball
{
    Vect2D Size;
    Vect2D_f Vitesse;
    Vect2D_f CUL;
    uint32_t Color;
};

struct Bat
{
    Vect2D Size;
    Vect2D_f CUL;
    uint32_t Color;
};

struct Mur
{
    Vect2D Center;
    uint32_t Color;
};


//Game Class and Methodes
class Game
{
    public:
    //Virtual pure methode
    virtual void Init(void) = 0;                                //Initialisation de la classe 
    virtual void Load(unsigned int nLvl) = 0;                   //chargement des fichiers niveau 
    virtual void Draw(void) = 0;                                //dessins du niveau 
    
    virtual void Update(StructManettes DataManettes) = 0;       //mise a jour du jeu 
    virtual void UpdateGame(StructManettes DataManettes) = 0;   //chargement des fichiers niveau 
    virtual void nextLevel(void)= 0;                            //
    virtual void WaitStart(StructManettes DataManettes)= 0;     //
    virtual void Fini(StructManettes DataManettes) = 0;         //
    virtual void Dead(StructManettes DataManettes) = 0;         //
    
    protected:
    uint32_t m_ColorLivePl1,  m_ColorLivePl2;                   //ColorLive Player1 and Player2
    Game_State m_State, m_State_Last, m_State_Next;             //Curent Last and Next state
    
    DisplayManager* m_pDisplayManager;                          //
    MyDFPlayer* m_pAudio;                                       //
    
    private:
};

#endif
