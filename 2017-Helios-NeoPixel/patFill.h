#include "pat.h"

// patFill:
//      clear the strip with a single color,
//      pattern is complete after numLoops... Work is only performed
//      in the first loop, the remaining loops represet delay (hold).

class patFill : public pat
{
private:
    CRGB m_c1;
    int m_maxLoops, m_currentLoops;

public:
    patFill(class Adafruit_NeoPixel &p, CRGB c1, int maxLoops):
        pat(p),
        m_c1(c1),
        m_maxLoops(maxLoops),
        m_currentLoops(0)
    {}

    virtual void loop(int loopCounter)
    {
        if(m_currentLoops == 0)
        {
            for (int i = 0; i < m_pixels.numPixels(); i++)
            {
                m_pixels.setPixelColor(i, m_c1);
            }
            m_pixels.show();
        }
        m_currentLoops++;
    }
    virtual bool done()
    {
        if(m_currentLoops < m_maxLoops)
            return false;
        else
        {
            m_currentLoops = 0;
            return true;
        }
    }
};
