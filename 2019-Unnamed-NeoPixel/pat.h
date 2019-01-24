#ifndef pat_h
#define pat_h

typedef uint32_t CRGB;

class pat
{
public:
    pat(class Adafruit_NeoPixel &strip) : m_pixels(strip) {}
    virtual ~pat() {};

    virtual void loop(int loopCounter) {};
    virtual bool done() { return true; }

protected:
    class Adafruit_NeoPixel &m_pixels;
};

#endif
