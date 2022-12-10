#include "../touch_button.hpp"
#include "communicator.hpp"
#include "indication.hpp"
#include "lock.hpp"

Cryptographer cryptographer;

Button button(4);
Button update_key_button(5);
Communicator communicator(9, 10, cryptographer);
Lock central_lock(8);

Indicator yellow(6);
Indicator red(7);
Indicator green(8);

bool SuspiciousActivityDetected = false;

void setup() 
{
    Serial.begin(9600);
    Serial.println("Car started");
    communicator.InitRadio(Communicator::Device::Car);
}

void loop()
{
    if (update_key_button.Check() == Button::Event::Pressed)
        communicator.NextKey();

    auto comm_event = Communicator::Event::None;

    if (not SuspiciousActivityDetected)
    {
        auto button_event = button.Check();
        if (button_event == Button::Event::Pressed)
            communicator.InitAutoOpen();
    }

    comm_event = communicator.Check();

    switch (comm_event)
    {
        case Communicator::Event::Timeout:
            yellow.Flash(3, 200, 200);
            break;

        case Communicator::Event::Open:
            central_lock.Switch();
            red.Flash(0, 0, 0);
            //communicator.SendStatusOpened();
            SuspiciousActivityDetected = false;
            break;
        
        case Communicator::Event::Confirmed:
            central_lock.Switch();
            red.Flash(0, 0, 0);
            //communicator.SendStatusOpened();
            SuspiciousActivityDetected = false;
            break;

        case Communicator::Event::Suspicion:
            red.Flash(-1, 500, 500);
            SuspiciousActivityDetected = true;
            //communicator.SendStatusSuspicious();
        
        default:
            break;
    }

    yellow.Process();
    red.Process();
    green.Process();
}
