#ifndef _TEXT_H_
#define _TEXT_H_

#include <Arduino.h>

struct Letter
{
    public:
    Vect2D Size;
    int Pixel_Size;
    Vect2D* Pixel;
};

static const Letter Letter_0 = Letter{{4,5}, 10, new Vect2D[10]{{1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {3,2}, {0,3}, {3,3}, {1,4}, {2,4}}}; 
static const Letter Letter_1 = Letter{{3,5}, 7 , new Vect2D[7] {{2,0}, {1,1}, {2,1}, {0,2}, {2,2}, {2,3}, {2,4}}}; 
static const Letter Letter_2 = Letter{{4,5}, 10, new Vect2D[10]{{1,0}, {2,0}, {0,1}, {3,1}, {2,2}, {1,3}, {0,4}, {1,4}, {2,4}, {3,4}}}; 
static const Letter Letter_3 = Letter{{4,5}, 11, new Vect2D[11]{{0,0}, {1,0}, {2,0}, {3,1}, {0,2}, {1,2}, {2,2}, {3,3}, {0,4}, {1,4}, {2,4}}}; 
static const Letter Letter_4 = Letter{{4,5}, 10, new Vect2D[10]{{0,0}, {3,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {3,2}, {3,3}, {3,4}}}; 
static const Letter Letter_5 = Letter{{4,5}, 12, new Vect2D[12]{{0,0}, {1,0}, {2,0}, {3,0}, {0,1}, {0,2}, {1,2}, {2,2}, {3,3}, {0,4}, {1,4}, {2,4}}}; 
static const Letter Letter_6 = Letter{{4,5}, 11, new Vect2D[11]{{1,0}, {1,0}, {2,0}, {0,1}, {0,2}, {1,2}, {2,2}, {0,3}, {3,3}, {1,4}, {2,4}}};  
static const Letter Letter_7 = Letter{{4,5}, 8 , new Vect2D[8] {{0,0}, {1,0}, {2,0}, {3,0}, {3,1}, {2,2}, {1,3}, {0,4}}};  
static const Letter Letter_8 = Letter{{4,5}, 10, new Vect2D[10]{{1,0}, {2,0}, {0,1}, {3,1}, {1,2}, {2,2}, {0,3}, {3,3}, {1,4}, {2,4}}};  
static const Letter Letter_9 = Letter{{4,5}, 11, new Vect2D[11]{{1,0}, {2,0}, {0,1}, {0,1}, {3,1}, {1,2}, {2,2}, {3,2}, {3,3}, {1,4}, {2,4}}};    

static const Letter Letter_A = Letter{{4,5}, 12, new Vect2D[12]{{1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {3,2}, {0,3}, {3,3}, {0,4}, {3,4}}};    
static const Letter Letter_B = Letter{{4,5}, 13, new Vect2D[13]{{0,0}, {1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {0,3}, {3,3}, {0,4}, {1,4}, {2,4}}};
static const Letter Letter_C = Letter{{4,5}, 9,  new Vect2D[9] {{1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {0,3}, {3,3}, {1,4}, {2,4}}};
static const Letter Letter_D = Letter{{4,5}, 12, new Vect2D[12]{{0,0}, {1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {3,2}, {0,3}, {3,3}, {0,4}, {1,4}, {2,4}}};
static const Letter Letter_E = Letter{{4,5}, 13, new Vect2D[13]{{0,0}, {1,0}, {2,0}, {3,0}, {0,1}, {0,2}, {1,2}, {2,2}, {0,3}, {0,4}, {1,4}, {2,4}, {3,4}}};
static const Letter Letter_F = Letter{{4,5}, 10, new Vect2D[10]{{0,0}, {1,0}, {2,0}, {3,0}, {0,1}, {0,2}, {1,2}, {2,2}, {0,3}, {0,4}}};
static const Letter Letter_G = Letter{{4,5}, 11, new Vect2D[11]{{1,0}, {2,0}, {3,0}, {0,1}, {0,2}, {2,2}, {3,2}, {0,3}, {3,3}, {1,4}, {2,4}}};
static const Letter Letter_H = Letter{{4,5}, 12, new Vect2D[12]{{0,0}, {3,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {3,2}, {0,3}, {3,3}, {0,4}, {3,4}}};
static const Letter Letter_I = Letter{{1,5}, 5,  new Vect2D[5] {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}}};
static const Letter Letter_J = Letter{{4,5}, 7,  new Vect2D[7] {{3,0}, {3,1}, {3,2}, {0,3}, {3,3}, {1,4}, {2,4}}};
static const Letter Letter_K = Letter{{4,5}, 10, new Vect2D[10]{{0,0}, {3,0}, {0,1}, {2,1}, {0,2}, {1,2}, {0,3}, {2,3}, {0,4}, {3,4}}};
static const Letter Letter_L = Letter{{3,5}, 7,  new Vect2D[7] {{0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {1,4}, {2,4}}};
static const Letter Letter_M = Letter{{5,5}, 13, new Vect2D[13]{{0,0}, {4,0}, {0,1}, {1,1}, {3,1}, {4,1}, {0,2}, {2,2}, {4,2}, {0,3}, {4,3}, {0,4}, {4,4}}};
static const Letter Letter_N = Letter{{5,5}, 13, new Vect2D[13]{{0,0}, {4,0}, {0,1}, {1,1}, {4,1}, {0,2}, {2,2}, {4,2}, {0,3}, {3,3}, {4,3}, {0,4}, {4,4}}};
static const Letter Letter_O = Letter{{4,5}, 10, new Vect2D[10]{{1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {3,2}, {0,3}, {3,3}, {1,4}, {2,4}}};
static const Letter Letter_P = Letter{{4,5}, 10, new Vect2D[10]{{0,0}, {1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {0,3}, {0,4}}};
static const Letter Letter_Q = Letter{{5,6}, 13, new Vect2D[13]{{1,0}, {2,0}, {3,0}, {0,1}, {4,1}, {0,2}, {2,2}, {4,2}, {0,3}, {3,3}, {1,4}, {2,4}, {4,4}}};
static const Letter Letter_R = Letter{{4,5}, 13, new Vect2D[13]{{0,0}, {1,0}, {2,0}, {0,1}, {3,1}, {0,2}, {1,2}, {2,2}, {0,3}, {2,3}, {0,4}, {3,4}}};
static const Letter Letter_S = Letter{{4,5}, 10, new Vect2D[10]{{1,0}, {2,0}, {3,0}, {0,1}, {1,2}, {2,2}, {3,3}, {0,4}, {1,4}, {2,4}}};
static const Letter Letter_T = Letter{{3,5}, 7,  new Vect2D[7] {{0,0}, {1,0}, {2,0}, {1,1}, {1,2}, {1,3}, {1,4}}};
static const Letter Letter_U = Letter{{4,5}, 10, new Vect2D[10]{{0,0}, {3,0}, {0,1}, {3,1}, {0,2}, {3,2}, {0,3}, {3,3}, {1,4}, {2,4}}};
static const Letter Letter_V = Letter{{5,5}, 9,  new Vect2D[9] {{0,0}, {4,0}, {0,1}, {4,1}, {1,2}, {3,2}, {1,3}, {3,3}, {2,4}}};
static const Letter Letter_W = Letter{{5,5}, 13, new Vect2D[13]{{0,0}, {4,0}, {0,1}, {2,1}, {4,1}, {0,2}, {2,2}, {4,2}, {0,3}, {2,3}, {4,3}, {1,4}, {3,4}}};
static const Letter Letter_X = Letter{{5,5}, 9,  new Vect2D[9] {{0,0}, {4,0}, {1,1}, {3,1}, {2,2}, {1,3}, {3,3}, {0,4}, {4,4}}};
static const Letter Letter_Y = Letter{{5,5}, 7,  new Vect2D[7] {{0,0}, {4,0}, {1,1}, {3,1}, {2,2}, {2,3}, {2,4}}};
static const Letter Letter_Z = Letter{{3,5}, 9,  new Vect2D[9] {{0,0}, {1,0}, {2,0}, {2,1}, {1,2}, {0,3}, {0,4}, {1,4}, {2,4}}};

static const Letter Letter_2p = Letter{{1,5}, 2,  new Vect2D[2] {{0,1},{0,3}}};
static const Letter Letter_Fleche = Letter{{3,5}, 9,  new Vect2D[9] {{0,0},{0,1},{1,1},{0,2},{1,2},{2,2},{0,3},{1,3},{0,4}}};
static const Letter Letter_Space = Letter{{3,5}, 0,  new Vect2D[2] {{0,1},{0,3}}};

//static Letter Happy_Smiley {{6,5}, 11, new Vect2D[11]{{2,1}, {2,2}, {4,1}, {4,2}, {0,3}, {1,4}, {2,5}, {3,5}, {4,5}, {5,4}, {6,3}}};
static const Letter Smiley = Letter{{8,8}, 31, new Vect2D[31]{{3,0}, {4,0}, {5,0}, {2,1}, {6,1}, {1,2}, {3,2}, {5,2}, {7,2}, {0,3}, {3,3}, {5,3}, {8,3}, {0,4}, {1,4}, {7,4}, {8,4}, {0,5}, {2,5}, {6,5}, {8,5}, {1,6}, {3,6}, {4,6}, {5,6}, {7,6}, {2,7}, {6,7}, {3,8}, {4,8}, {5,8}}} ;
static const Letter Very_Happy_Smiley = Letter{{6,5}, 16, new Vect2D[16]{{2,0}, {2,1}, {4,0}, {4,1}, {0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {6,3}, {1,4}, {5,4}, {2,5}, {3,5}, {4,5}}} ;

struct TextMatrice
{
    unsigned int nSize;
    Vect2D* Table;
};

class Text
{
    public:
    Text(char* Str_)
    {
        Matrice.Table = NULL;   
        Matrice.nSize = 0;  
         
        Write(Str_);
    }
    
    Text(int Int_)
    {           
        Matrice.Table = NULL;    
        Matrice.nSize = 0;  
          
        int nSize = 0;
        int nRest =  Int_;
        while(nRest>=10)
        {
            ++nSize;
            nRest /= 10;
        }
        nSize += 1;
                 
        char* Str =  new char[nSize+1]; 
        Str[nSize]= '\0';
        
        nRest =  Int_;
        Serial.print("-->"); 
        for(int ii = 0; ii < nSize; ++ii)
        {
            Str[ii] ='0' +  (Int_% (int)pow(10,nSize-ii) )/ pow(10,nSize-ii-1);
            Serial.println( (Int_% (int)pow(10,nSize-ii) )/ pow(10,nSize-ii-1) ); 
        }
        
        
             
        Write(Str);
        delete[] Str;

    }

    void Write(char* Str)
    {      
        //recherche taille de la nouvelle matrice a ecrire
        Matrice.nSize = 0;
        unsigned int nIndice = 0;
        while(Str[nIndice] != '\0')
        {
            const Letter* letter = SelectLetter(Str[nIndice]);
            Matrice.nSize += letter->Pixel_Size;            
            ++nIndice;
        }
        
        //on cree la nouvelle matrice
        Matrice.Table = new Vect2D[Matrice.nSize];
        for(unsigned int ii = 0; ii < Matrice.nSize; ++ii)
        {
           Matrice.Table[ii].X = 0;
           Matrice.Table[ii].Y = 0;
        }
        //on remplie la nouvelle matrice
        nIndice = 0;
        unsigned int nIndeiceXMatrice = 0;
        unsigned int nIndiceLetterPixel = 0;
        while(Str[nIndice] != '\0')
        {
            const Letter* letter = SelectLetter(Str[nIndice]);
            for(unsigned int ii = 0; ii < letter->Pixel_Size; ++ii)
            {
                Matrice.Table[nIndiceLetterPixel] = letter->Pixel[ii];
                Matrice.Table[nIndiceLetterPixel].X += nIndeiceXMatrice;
                ++nIndiceLetterPixel;
            }

            nIndeiceXMatrice += letter->Size.X+1;
            ++nIndice;
            
        }

    }
    
    ~Text()
    {
        if(Matrice.Table != NULL)delete[] Matrice.Table;
    }
    
    void Draw(const Vect2D CUL, const uint32_t Color, DisplayManager* pDisplayManager)const
    {
       for(int ii = 0; ii < Matrice.nSize; ++ii)
       {
            pDisplayManager->SetPixel(Matrice.Table[ii].X + CUL.X, Matrice.Table[ii].Y + CUL.Y, Color );   
       }
    }

    const Letter* SelectLetter(char letter)
    {
        const Letter* ret = &Letter_A;
        switch(letter)
        {
          case '0':ret = &Letter_0; break;
          case '1':ret = &Letter_1; break;
          case '2':ret = &Letter_2; break;
          case '3':ret = &Letter_3; break;
          case '4':ret = &Letter_4; break;
          case '5':ret = &Letter_5; break;
          case '6':ret = &Letter_6; break;
          case '7':ret = &Letter_7; break;
          case '8':ret = &Letter_8; break;
          case '9':ret = &Letter_9; break;
          
          case ':':ret = &Letter_2p; break;
          
          case 'A':ret = &Letter_A; break;
          case 'B':ret = &Letter_B; break;
          case 'C':ret = &Letter_C; break;
          case 'D':ret = &Letter_D; break;
          case 'E':ret = &Letter_E; break;
          case 'F':ret = &Letter_F; break;
          case 'G':ret = &Letter_G; break;
          case 'H':ret = &Letter_H; break;
          case 'I':ret = &Letter_I; break;
          case 'J':ret = &Letter_J; break;
          case 'K':ret = &Letter_K; break;
          case 'L':ret = &Letter_L; break;
          case 'M':ret = &Letter_M; break;
          case 'N':ret = &Letter_N; break;
          case 'O':ret = &Letter_O; break;
          case 'P':ret = &Letter_P; break;
          case 'Q':ret = &Letter_Q; break;
          case 'R':ret = &Letter_R; break;
          case 'S':ret = &Letter_S; break;
          case 'T':ret = &Letter_T; break;
          case 'U':ret = &Letter_U; break;
          case 'V':ret = &Letter_V; break;
          case 'W':ret = &Letter_W; break;
          case 'X':ret = &Letter_X; break;
          case 'Y':ret = &Letter_Y; break;
          case 'Z':ret = &Letter_Z; break;
          
          case '+':ret = &Very_Happy_Smiley; break;
          case '-':ret = &Letter_Fleche; break;
          case '_':ret = &Letter_Space; break;
          
          
        }
        return ret;
    }
    
    private:    
    TextMatrice Matrice;
    
};

#endif
