#define ATTACK_IMITATION

#include "../touch_button.hpp"
#include "../cryptographer.hpp"
#include "communicator.hpp"
#include "indication.hpp"

#include "attack_imitator.hpp"

Cryptographer cryptographer;

Button button(7);
Communicator communicator(9, 10, cryptographer);

Indicator yellow(8);

void setup() 
{
    Serial.begin(9600);
    Serial.println("Key started");
    communicator.InitRadio(Communicator::Device::Key1);
}

void loop() 
{
    auto comm_event = communicator.Check();
    auto button_event = button.Check();

    if (comm_event == Communicator::Event::OpenRequest)
        yellow.Flash(1, 250, 0);

    else if (button_event == Button::Event::Pressed)
        communicator.SendOpen();

    yellow.Process();
}
