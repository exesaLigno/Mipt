#pragma once

class Lock
{
private:
    int Pin = 0;
    bool Opened = false;

public:
    Lock(int pin) : Pin(pin) { }
    ~Lock() { }

    void Switch() 
    {
        Opened = not Opened;
        
        if (Opened) analogWrite(Pin, 900);
        else analogWrite(Pin, 0);
    }
};
