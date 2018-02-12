#include "pat.h"

// patExpandMiddle:
//  starts with solid c2, then fills c1 starting at the center
//  one cycle is complete when entire strip is flooded with c1.

class patExpandMiddle : public pat
{
private:
    CRGB m_c1, m_c2;
    int m_maxCount, m_cycleCount, m_subcycleCount;

public:
    patExpandMiddle(class Adafruit_NeoPixel &p, CRGB c1, CRGB c2, int maxCount):
        pat(p),
        m_c1(c1),
        m_c2(c2),
        m_maxCount(maxCount),
        m_cycleCount(0),
        m_subcycleCount(0)
    {}

    virtual void loop(int loopCounter)
    {
        int nleds = m_pixels.numPixels();
        if(m_cycleCount == 0)
        {
            // fill strip with c2
            for(int i=0;i<nleds;i++)
            {
                m_pixels.setPixelColor(i, m_c2);
            }
            m_cycleCount++; // one cycle complete
        }
        else
        {
            if(m_subcycleCount < 16)
            {
                if(m_subcycleCount < 8)
                    insertMiddle(m_c1);
                else
                    insertMiddle(m_c2);
                m_subcycleCount++;
            }
            else
            {
                m_subcycleCount = 0;
                m_cycleCount++; // one cycle complete
            }
        }
        m_pixels.show();
    }

    void insertMiddle(CRGB cIn)
    {
        int nleds = m_pixels.numPixels();
        for(int i=0;i<(nleds/2-1);i++)
        {
            m_pixels.setPixelColor(i, m_pixels.getPixelColor(i+1));
            m_pixels.setPixelColor(nleds-i-1, m_pixels.getPixelColor(nleds-i-2));
        }
        if(nleds & 1)
            m_pixels.setPixelColor((nleds-1)/2, cIn);
        else
        {
            m_pixels.setPixelColor(nleds/2 - 1, cIn);
            m_pixels.setPixelColor(nleds/2, cIn);
        }
    }

    virtual bool done()
    {
        if(m_cycleCount < m_maxCount)
            return false;
        else
        {
            m_cycleCount = 0;
            return true;
        }
    }
};
