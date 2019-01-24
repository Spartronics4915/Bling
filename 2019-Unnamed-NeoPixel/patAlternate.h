#include "pat.h"

// patAlternate:
//  fills strip with two alternating colors

class patAlternate : public pat
{
private:
    CRGB m_c1, m_c2;
    int m_maxCount, m_currentCount;
    int m_delayCount;
public:
    patAlternate(class Adafruit_NeoPixel &p, CRGB c1, CRGB c2, int maxCount, int delayCount):
        pat(p),
        m_c1(c1),
        m_c2(c2),
        m_maxCount(maxCount),
        m_currentCount(0),
        m_delayCount(delayCount)
    {}

    virtual void loop(int loopCounter)
    {
        int nleds = m_pixels.numPixels();
        if(loopCounter % m_delayCount == 0)
        {
          for(int i=0;i<nleds;i++)
          {
              if((i+m_currentCount)&1)
                  m_pixels.setPixelColor(i, m_c1);
              else
                  m_pixels.setPixelColor(i, m_c2);
          }
          
          m_pixels.show();
          m_currentCount++; // one cycle complete
        }
        }

    virtual bool done()
    {
        if(m_currentCount < m_maxCount)
            return false;
        else
        {
            m_currentCount = 0;
            return true;
        }
    }
};
