#pragma once

#include "../radio_communicator.hpp"
#include "../secret.hpp"
#include "../random.hpp"

class Communicator : public RadioCommunicator
{
public:

    enum class Event : byte
    {
        None, 
        Open, /// Command directly from key (pressed 'open' button)
        Timeout, /// Auto-open time out
        Suspicion, /// Auto-open reply delay detected
        Confirmed /// Auto-open reply received without delay
    };

private:

    bool AutoOpenRequested = false;
    unsigned long AutoOpenRequestTime = 0;
    unsigned int AutoOpenLastQueryId = 0;
    static int Timeout;

    bool NextKeyRequested = false;
    unsigned long NextKeyRequestTime = 0;
    unsigned int NextKeyLastQueryId = 0;
    unsigned long long NextKeyRequestRandomValueHash = 0;

public:
    Communicator(int ce_pin, int cs_pin, Cryptographer crypt) : RadioCommunicator(ce_pin, cs_pin, crypt) { }
    ~Communicator() { }

    Event Check()
    {
        Message income = Listen();

        if (NextKeyRequested and millis() - NextKeyRequestTime > Timeout)   // If next key ack timeout exceeded: declining next key
        {
            FlushNextKey();
            Crypt.DeclineNextKey();
        }

        else if (NextKeyRequested and !income.IsEmpty() and                     // If acknowlegment received, processing it
                income.GetCmd() == Message::Command::UpdateKeyAcknowlegment)
        {
            if (income.GetPayload() == NextKeyRequestRandomValueHash and    // If all correct in acknowlegment - accepting it
                income.GetQueryId() == NextKeyLastQueryId)
            {
                FlushNextKey();
                Crypt.AcceptNextKey();
            }

            else    // If something is not correct - declinig next key
            {
                FlushNextKey();
                Crypt.DeclineNextKey();
            }
        }

        if (!income.IsEmpty() and income.GetCmd() == Message::Command::Open)
        {
            if (income.GetPayload() == SECRET)
                return Event::Open;
            else
                return Event::Suspicion;
        }

        else if (AutoOpenRequested and millis() - AutoOpenRequestTime > Timeout)
        {
            FlushAutoOpen();
            return Event::Timeout;
        }

        else if (AutoOpenRequested and !income.IsEmpty() and 
                income.GetCmd() == Message::Command::AutoOpenConfirmation)
        {
            if (millis() - income.GetTimestamp() < 100 and 
                income.GetPayload() == SECRET and 
                income.GetQueryId() == AutoOpenLastQueryId)
            {
                FlushAutoOpen();
                return Event::Confirmed;
            }

            else
            {
                FlushAutoOpen();
                return Event::Suspicion;
            }
        }

        return Event::None;
    }

    void InitAutoOpen()
    {
        Message request = Message(Message::Command::AutoOpenRequest);

        AutoOpenRequested = true;
        AutoOpenRequestTime = millis();
        AutoOpenLastQueryId = request.GetQueryId();

        SendMessage(request);
    }

    void SendStatusOpened()
    {
        Message status = Message(Message::Command::StatusOpen);
        SendMessage(status);
    }

    void SendStatusClosed()
    {
        Message status = Message(Message::Command::StatusClosed);
        SendMessage(status);
    }

    void SendStatusSuspicious()
    {
        Message status = Message(Message::Command::StatusSuspiciousMode);
        SendMessage(status);
    }

    void NextKey()
    {
        unsigned long long random_value = Random<unsigned long long>();
        Message next_key_request = Message(Message::Command::UpdateKey, random_value);

        NextKeyRequested = true;
        NextKeyRequestTime = millis();
        NextKeyLastQueryId = next_key_request.GetQueryId();

        SendMessage(next_key_request);

        Crypt.ProbeNextKey();
        NextKeyRequestRandomValueHash = Crypt.GetHash(random_value);
    }

    void FlushAutoOpen()
    {
        AutoOpenRequested = false;
        AutoOpenRequestTime = 0;
        AutoOpenLastQueryId = 0;
    }

    void FlushNextKey()
    {
        NextKeyRequested = false;
        NextKeyRequestTime = 0;
        NextKeyLastQueryId = 0;
        NextKeyRequestRandomValueHash = 0;
    }
};

int Communicator::Timeout = 2000;
