#include "pat.h"

// patCogs:
//      write a strip made of two colors

#define COG_SIZE 8
class patCogs : public pat
{
private:
    CRGB m_c1, m_c2;
    int m_currentCount = 0;
    int m_maxCount;
public:
    patCogs(class Adafruit_NeoPixel &p, CRGB c1, CRGB c2, int maxCount) :
        pat(p),
        m_c1(c1),
        m_c2(c2),
        m_maxCount(maxCount)
    {}

    virtual void loop(int loopCounter)
    {
        for (int i = 0; i < m_pixels.numPixels(); i++)
        {
            if (((int) (i / COG_SIZE)) & 1)
            {
                m_pixels.setPixelColor(i, m_c1);
            }
            else
            {
                m_pixels.setPixelColor(i, m_c2);
            }
        }
        m_pixels.show();
        m_currentCount++;
    }
    virtual bool done()
    {
        //return true;
        if(m_currentCount < m_maxCount)
            return false;
        else
        {
            m_currentCount = 0;
            return true;
        }
    }
};
