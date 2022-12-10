#pragma once

#include "../radio_communicator.hpp"
#include "../secret.hpp"
#include "attack_imitator.hpp"

class Communicator : public RadioCommunicator
{
public:

    enum class Event 
    {
        None,
        OpenRequest,
        CarOpen,
        CarClosed,
        CarInSuspiciousMode
    };

private:


public:
    Communicator(int ce_pin, int cs_pin, Cryptographer crypt) : RadioCommunicator(ce_pin, cs_pin, crypt) { }
    ~Communicator() { }

    Event Check()
    {
        Message income = Listen();
        if (!income.IsEmpty())
        {
            switch (income.GetCmd())
            {
                case Message::Command::AutoOpenRequest:
                    if (RadiusIncreasingAttack()) delay(200);
                    SendMessage(income.CreateReplyInQuery(Message::Command::AutoOpenConfirmation, KeyImitationAttack() ? FAKE_SECRET : SECRET));
                    return Event::OpenRequest;

                case Message::Command::UpdateKey:
                    Crypt.SetNextKey();
                    SendMessage(income.CreateReplyInQuery(Message::Command::UpdateKeyAcknowlegment, Crypt.GetHash(income.GetPayload())));
                
                default: 
                    return Event::None;

            }
        }

        return Event::None;
    }

    void SendOpen()
    {
        Message open = Message(Message::Command::Open, KeyImitationAttack() ? FAKE_SECRET : SECRET);
        SendMessage(open);
    }
};
