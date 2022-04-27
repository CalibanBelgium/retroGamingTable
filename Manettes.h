#ifndef _MANETTE_H_
#define _MANETTE_H_

#include <Arduino.h>


/* Constantes des bits de chaque bouton */
#define BTN_A       256
#define BTN_B       1
#define BTN_X       512
#define BTN_Y       2
#define BTN_SELECT  4
#define BTN_START   8
#define BTN_UP      16
#define BTN_DOWN    32
#define BTN_LEFT    64
#define BTN_RIGHT   128
#define BTN_L       1024
#define BTN_R       2048
#define NO_GAMEPAD  61440
 
struct Manette_Data
{
  uint16_t State_Current, State_Old;
  int nPinData;
};

class Manettes
{
    public:
    Manettes(const int nPinLatch, const int nPinClock, const int nPinDataManette_1, const int nPinDataManette_2)
    {
        m_nPinLatch = nPinLatch;
        m_nPinClock = nPinClock;
        m_DataManette_1.nPinData = nPinDataManette_1;
        m_DataManette_2.nPinData = nPinDataManette_2;
        
        pinMode(m_nPinLatch, OUTPUT);
        pinMode(m_nPinClock, OUTPUT);
        pinMode(m_DataManette_1.nPinData, INPUT); 
        pinMode(m_DataManette_2.nPinData, INPUT); 
    }
    ~Manettes()
    {
      //TODO
    }
    void Update(void)
    {
        m_DataManette_1.State_Old = m_DataManette_1.State_Current;
        m_DataManette_2.State_Old = m_DataManette_2.State_Current;
        /** recuperation de l'état de chaque bouton sous la forme d'un entier sur 16 bits. */
        /* 1 bouton = 1 bit */
        m_DataManette_1.State_Current = 0;
        m_DataManette_2.State_Current = 0;
       
        /* Capture de l'état courant des boutons */
        digitalWrite(PIN_LATCH, HIGH);
        digitalWrite(PIN_LATCH, LOW);
       
        /* Récupère l'état de chaque bouton (12 bits + 4 bits à "1") */
        for(byte i = 0; i < 16; ++i) 
        {       
          /* Lit l'état du bouton et décale le bit reçu pour former l'entier sur 16 bits final */
          m_DataManette_1.State_Current |= digitalRead(m_DataManette_1.nPinData) << i;       
          m_DataManette_2.State_Current |= digitalRead(m_DataManette_2.nPinData) << i;       
          /* Pulsation du signal d'horloge */
          digitalWrite(PIN_CLOCK, HIGH);
          digitalWrite(PIN_CLOCK, LOW);          
        } 
        m_DataManette_1.State_Current = ~m_DataManette_1.State_Current;
        m_DataManette_2.State_Current = ~m_DataManette_2.State_Current;     
    }
    uint16_t GetData1(void) { return m_DataManette_1.State_Current;}
    uint16_t GetData2(void) { return m_DataManette_2.State_Current;}
    StructManettes GetData(void) { StructManettes ret; ret.DataManette1 = m_DataManette_1.State_Current; ret.DataManette2 = m_DataManette_2.State_Current; return ret;}
    
    private:
    Manette_Data m_DataManette_1, m_DataManette_2;
    int m_nPinLatch, m_nPinClock;
};

#endif
