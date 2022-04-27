#ifndef _TIME_H_111
#define _TIME_H_111

#include <Arduino.h>
#include <Wire.h>
#include "Text.h"
#include "Config.h"

#define DS3231_I2C_ADDRESS 0x68

byte decToBcd(byte val)
{
  //return( (val/10*16) + (val%10) );
  return( (val/160) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  //return( (val/16*10) + (val%16) );
  return( (val/160) + (val%16) );
}

class Time  : public Game 
{
  public:
  Time(DisplayManager* pDisplayManager, MyDFPlayer* pAudio)
  {
      m_pDisplayManager = pDisplayManager; 
  }
  ~Time(){};

  void Update(StructManettes DataManettes)
  {     
      readDS3231time(m_nSecond, m_nMinute, m_nHour, m_nDayOfWeek, m_nDayOfMonth, m_nMonth, m_nYear); 

      Draw();
  }
  
  void nextLevel(void){}; //Not used
    
  void Draw(void) 
  {       
      char date[6] = {0};
      date[0] = (char)('0' + ((int)m_nHour/10));
      date[1] = (char)('0' + ((int)m_nHour - ((int)m_nHour/10)*10));
      date[2] = (char)(':');
      date[3] = (char)('0' + ((int)m_nMinute/10));
      date[4] = (char)('0' + ((int)m_nMinute - ((int)m_nMinute/10)*10));
      date[5] = (char)('\0');
      
      m_nY = (int)m_nMinute/2;
      m_nX = (int)m_nHour/3;

      m_pDisplayManager->OFF();
      Text txt0(date);
      txt0.Draw({m_nX,m_nY}, C_BLUE, m_pDisplayManager);
  }
  void Init(void){};   //Initialisation de la classe //fonction virtuelle pure  
  void Load(unsigned int nLvl){};   //chargement des fichiers niveau //fonction virtuelle pure  
  void WaitStart(StructManettes DataManettes){};   //chargement des fichiers niveau //fonction virtuelle pure  
  void Fini(StructManettes DataManettes){};   //chargement des fichiers niveau //fonction virtuelle pure  
  void Dead(StructManettes DataManettes){};   //chargement des fichiers niveau //fonction virtuelle pure  
  void UpdateGame(StructManettes DataManettes){};   //chargement des fichiers niveau //fonction virtuelle pure  
  
  void static readDS3231time(byte &nSecond_,  byte &nMinute_,  byte &nHour_,  byte &nDayOfWeek_,  byte &nDayOfMonth_,  byte &nMonth_,  byte &nYear_)
  {
      Wire.beginTransmission(DS3231_I2C_ADDRESS);
      Wire.write(0); // set DS3231 register pointer to 00h
      Wire.endTransmission();
      Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
      // request seven bytes of data from DS3231 starting from register 00h
      nSecond_ = bcdToDec(Wire.read() & 0x7f);
      nMinute_ = bcdToDec(Wire.read());
      nHour_ = bcdToDec(Wire.read() & 0x3f);
      nDayOfWeek_ = bcdToDec(Wire.read());
      nDayOfMonth_ = bcdToDec(Wire.read());
      nMonth_ = bcdToDec(Wire.read());
      nYear_ = bcdToDec(Wire.read());
  }
  
  private :
  byte m_nSecond, m_nMinute, m_nHour, m_nDayOfWeek, m_nDayOfMonth, m_nMonth, m_nYear;
  int m_nX, m_nY;


};


#endif
