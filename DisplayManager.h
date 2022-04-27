#ifndef _DISPLAYMANAGER_H_
#define _DISPLAYMANAGER_H_

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

class DisplayManager
{
    public:
/************************************************************************/
//constructeur
/************************************************************************/
    DisplayManager(const unsigned int nSizeX, const unsigned int nSizeY)
    {
        m_nSizeX = nSizeX;
        m_nSizeY = nSizeY; 
        m_nSize = m_nSizeX*m_nSizeY;
        
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_KHZ800  = 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400  = 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB     = Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB     = Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
        m_pStripColor = new Adafruit_NeoPixel(m_nSizeX*m_nSizeY, PIN_STRIPLED, NEO_GRB + NEO_KHZ800);   
        m_pStripColor->begin();  
    }
    
/************************************************************************/
//Destructeur
/************************************************************************/
    ~DisplayManager(void)
    {        
        delete m_pStripColor;
    }    

/************************************************************************/
//Updater
/************************************************************************/
    void Draw(void)
    {
        m_pStripColor->show();
    }
/************************************************************************/
//Methodes d'affichage
/************************************************************************/
    //OFF
    void OFF(void)
    {
        int nSize = m_nSizeX * m_nSizeY;
        for(int ii = 0; ii < nSize ; ++ii)
        {
            m_pStripColor->setPixelColor(ii,(uint32_t)0);  
        }
    }
/************************************************************************/
//Accessor
/************************************************************************/    
    //change la couleur de la zone demande, si possible
    void SetPixel(const unsigned int nCoorX, const unsigned int nCoorY, const uint32_t color){ SetPixel_(  nCoorX, nCoorY, color);}
    void SetPixel(const Vect2D nCoor,  const uint32_t color){ SetPixel_(nCoor.X, nCoor.Y, color);}
    void SetPixel(const unsigned int nCoorX, const unsigned int nCoorY, const ColorRGB color){ SetPixel_(nCoorX, nCoorY, Adafruit_NeoPixel::Color(color.R, color.G,color.B));}
    void SetPixel(const Vect2D nCoor,  const ColorRGB color){ SetPixel_(nCoor.X, nCoor.Y, Adafruit_NeoPixel::Color(color.R, color.G,color.B));}
    
    unsigned int GetSizeX(void)const {return m_nSizeX;};
    unsigned int GetSizeY(void)const {return m_nSizeY;};
        
    private:
    //variables
    unsigned int m_nSizeX, m_nSizeY, m_nSize;    //dimensions de la zone de la table en pixels
    Adafruit_NeoPixel* m_pStripColor;

    inline void SetPixel_(const unsigned int nCoorX, const unsigned int nCoorY, const uint32_t color)const
    {      
        if(nCoorX >= m_nSizeX || nCoorY >= m_nSizeY) return;   
        if(nCoorY%2 == 0)
        { 
            m_pStripColor->setPixelColor(nCoorY*SIZE_ALLSTRIP_X + nCoorX, color);     
        }
        else
        {
            m_pStripColor->setPixelColor(nCoorY*SIZE_ALLSTRIP_X + SIZE_ALLSTRIP_X - 1 - nCoorX, color);     
        }          
    } 
};
#endif
