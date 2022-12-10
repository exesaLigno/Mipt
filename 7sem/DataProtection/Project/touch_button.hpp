#pragma once

class Button
{
public:
    enum class Event : byte
    { 
        None, 
        Withheld, 
        Pressed, 
        Released 
    };

private:
    int Pin = 0;
    bool IsPressed = false;

public:
    Button(int pin) : Pin(pin) { }
    ~Button() { }

    Event Check()
    {
        bool pressed = digitalRead(Pin);

        //Serial.println(pressed);

        Event event = Event::None;

        if (pressed)
        {
            if (IsPressed) event = Event::Withheld;
            else event = Event::Pressed;
        }

        else
        {
            if (IsPressed) event = Event::Released;
            else event = Event::None;
        }

        IsPressed = pressed;

        return event;
        //return Event::None;
    }
};
