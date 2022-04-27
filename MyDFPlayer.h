#ifndef _MYDFPLAYER_H_
#define _MYDFPLAYER_H_

#include <Arduino.h>
#include "Config.h"

enum AUDIO_SELECTION
{
  eAudion_Selection_NULL = 0,
  eAudion_Selection_Menu,
  eAudion_Selection_PONG_Bat,
  eAudion_Selection_PONG_Dead,
  eAudion_Selection_CASSEBRIQUE_Bat,
  eAudion_Selection_CASSEBRIQUE_Brique,
  eAudion_Selection_CASSEBRIQUE_Mur,
  eAudion_Selection_CASSEBRIQUE_None,
  eAudion_Selection_CASSEBRIQUE_Dead,
  eAudion_Selection_CASSEBRIQUE_Start,
  eAudion_Selection_CASSEBRIQUE_BallePerdue,
  eAudion_Selection_CASSEBRIQUE_LvlUp,
  eAudion_Selection_TETRIS_Music,
  
  eAudion_Selection_SIZE  
};

class MyDFPlayer
{
  public:
  MyDFPlayer()  { Serial1.begin(9600); };
  ~MyDFPlayer()  {  };

//Volume
  void SetVolume(unsigned int nValue){WriteCmd(0x06, 0x00, 0x00, min(nValue,30));delay(200);};
  void UpVolume(void){WriteCmd(0x04, 0x00, 0x00, 0x00);delay(200);};
  void DownVolume(void){WriteCmd(0x05, 0x00, 0x00, 0x00);delay(200);};
  
  void Play(AUDIO_SELECTION nIndiceAudio){WriteCmd(0x03, 0x00, 0x00, (char)nIndiceAudio);};
  void Reset(void){WriteCmd(0x0C, 0x00, 0x00, 0x00);SetVolume(0);};
  void Pause(void){WriteCmd(0x0E, 0x00, 0x00, 0x00);};

  void  PlayLoop(AUDIO_SELECTION nIndiceAudio)
  {
    WriteCmd(0x03, 0x00, 0x00, (char)nIndiceAudio);
    delay (200);
    WriteCmd(0x19, 0x00, 0x00, 0x00);
    delay (200);
  };
  void  StopLoop(void)
  {
    WriteCmd(0x19, 0x00, 0x00, 0x01);
    delay (200);
  };
     
  private:
  char m_BufferCmd[10]= {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
  uint16_t nCheckSum;
  void WriteCmd(char CMD, char FeedBack, char Para1, char Para2)
  {
    m_BufferCmd[3] = CMD;
    m_BufferCmd[4] = FeedBack;
    m_BufferCmd[5] = Para1;
    m_BufferCmd[6] = Para2;
    
    nCheckSum = 0xFF + 0x06 + CMD + FeedBack + Para1 + Para2;  
    nCheckSum = -nCheckSum;
    
    m_BufferCmd[7] = (uint8_t)(nCheckSum>>8) ;
    m_BufferCmd[8] = (uint8_t)nCheckSum;
        
    Serial1.write(m_BufferCmd, 10);
  };

};
#endif
