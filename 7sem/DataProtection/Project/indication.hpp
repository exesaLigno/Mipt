#pragma once

class Indicator
{
private:
    int Pin = 0;
    int FlashesLeft = 0;
    bool Enabled = false;
    unsigned long LastUpdate = 0;
    int OnTime = 0;
    int OffTime = 0;

    bool Active = false;

public:
    Indicator(int pin) : Pin(pin) { }
    ~Indicator() { }

    void Process()
    {
        if (Enabled && millis() - LastUpdate >= OnTime)
        {
            Enabled = false;
            analogWrite(Pin, 0);

            LastUpdate = millis();
            
            if (FlashesLeft == 0)
                Clear();
        }

        else if (not Enabled and FlashesLeft != 0 and millis() - LastUpdate >= OffTime)
        {
            Enabled = true;

            if (FlashesLeft > 0) FlashesLeft--;
            analogWrite(Pin, 700);

            LastUpdate = millis();
        }
    }

    void Flash(int times, int on_time, int off_time)
    {
        FlashesLeft = times;
        OnTime = on_time;
        OffTime = off_time;
        Active = true;

        Process();
    }

private:
    void Clear()
    {
        FlashesLeft = 0;
        LastUpdate = 0;
        Enabled = false;
        OnTime = 0;
        OffTime = 0;

        Active = false;
    }
};
