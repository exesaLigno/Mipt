#pragma once

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include "cryptographer.hpp"


// #include </home/exesa_ligno/Documents/Programming/Arduino/nRF24L01/RF24-master/nRF24L01.h>
// #include </home/exesa_ligno/Documents/Programming/Arduino/nRF24L01/RF24-master/RF24.h>

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

class RadioCommunicator
{
public:

    enum class Device : byte
    {
        None,
        Car,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5
    };

    class Message
    {
    public:
        enum class Command : byte
        {
            None,
            Open,
            AutoOpenConfirmation,
            AutoOpenRequest,
            StatusOpen,
            StatusClosed,
            StatusSuspiciousMode,
            UpdateKey,
            UpdateKeyAcknowlegment
        };

    private:
        unsigned int QueryId = 0;
        Command Cmd = Command::None;
        unsigned long Timestamp = 0;
        unsigned long long Payload = 0;

        static unsigned int QueryCounter;
        static unsigned int GetNewQueryId()
        {
            QueryCounter += random(1, 10);
            return QueryCounter;
        }

        bool Empty = false;

    public:
        Message() { }
        Message(Command cmd) : QueryId(GetNewQueryId()), Cmd(cmd), Timestamp(millis()), Payload(random()) { }
        Message(Command cmd, unsigned long long payload) : QueryId(GetNewQueryId()), Cmd(cmd), Timestamp(millis()), Payload(payload) { }
        ~Message() { }

        static int ByteSize()
        {
            return sizeof(QueryId) + sizeof(Cmd) + sizeof(Timestamp) + sizeof(Payload);
        }

        void ToBytes(byte* bytes)
        {
            memcpy(bytes, (byte*) &QueryId, sizeof(QueryId));
            memcpy(bytes + sizeof(QueryId), (byte*) &Cmd, sizeof(Cmd));
            memcpy(bytes + sizeof(QueryId) + sizeof(Cmd), (byte*) &Timestamp, sizeof(Timestamp));
            if (Payload != 0) memcpy(bytes + sizeof(QueryId) + sizeof(Cmd) + sizeof(Timestamp), (byte*) &Payload, sizeof(Payload));
        }

        static Message FromBytes(byte* bytes)
        {
            Message readed;
            readed.QueryId = *((unsigned int*) bytes);
            readed.Cmd = *((Command*) (bytes + sizeof(QueryId)));
            readed.Timestamp = *((unsigned long*) (bytes + sizeof(QueryId) + sizeof(Cmd)));
            readed.Payload = *((unsigned long long*) (bytes + sizeof(QueryId) + sizeof(Cmd) + sizeof(Timestamp)));

            if (readed.Cmd == Command::None) readed.Empty = true;

            return readed;
        }

        bool IsEmpty()
        {
            return Empty;
        }

        Message CreateReplyInQuery(Command cmd, unsigned long long payload)
        {
            Message reply(cmd, payload);
            reply.QueryId = QueryId;
            reply.Timestamp = Timestamp;
            return reply;
        }

        Command GetCmd() { return Cmd; }
        unsigned long long GetPayload() { return Payload; }
        unsigned int GetQueryId() { return QueryId; }
        unsigned long GetTimestamp() { return Timestamp; }

        // #ifdef ENABLE_SERIAL
        // void ToSerial()
        // {
        //     Serial.println("Message:");
        //     Serial.print("QueryID: ");
        //     Serial.println(QueryId);
        //     Serial.print("Cmd: ");
        //     Serial.println((byte) Cmd);
        //     Serial.print("Timestamp: ");
        //     Serial.println(Timestamp);
        //     Serial.println("Payload: ");
        //     //Serial.println(Payload);
        // }
        // #endif
    };

protected:

    RF24 Radio;
    Cryptographer Crypt;

public:
    RadioCommunicator(int ce_pin, int cs_pin, Cryptographer crypt) : Radio(RF24(ce_pin, cs_pin)), Crypt(crypt) { }
    ~RadioCommunicator() { }



    void InitRadio(Device device)
    {
        Radio.begin();
        Radio.setAutoAck(1);
        //Radio.setRetries(0, 25);
        //Radio.enableAckPayload();
        Radio.setPayloadSize(32);

        switch (device)
        {
            case Device::Car:
                Radio.openWritingPipe(0xAABBCCDD11LL);
                Radio.openReadingPipe(1, 0xAABBCCDD22LL);
                break;

            case Device::Key1:
                Radio.openWritingPipe(0xAABBCCDD22LL);
                Radio.openReadingPipe(1, 0xAABBCCDD11LL);
                break;

            default:
                break;
        }

        Radio.setChannel(0x60);

        Radio.setPALevel(RF24_PA_MAX);
        Radio.setDataRate(RF24_250KBPS);
        
        Radio.powerUp();
        // if (device == Device::Key1) Radio.startListening();
        // else Radio.stopListening();
        Radio.startListening();

        #ifdef ENABLE_SERIAL
        //printf_begin();
        //Radio.printDetails();
        #endif
    }

    Message SendMessageWaitingReply(Message message, bool fast = false)
    {
        byte m_bytes[Message::ByteSize()] { 0 };
        byte ack_bytes[Message::ByteSize()] { 0 };
        message.ToBytes(m_bytes);
        Radio.stopListening();
        if (Radio.write(m_bytes, Message::ByteSize()))
        {
            while (Radio.available())
                Radio.read(ack_bytes, Message::ByteSize());
        }
        Radio.startListening();
        return Message::FromBytes(ack_bytes);
    }

    void SendMessage(Message message)
    {
        byte m_bytes[Message::ByteSize()] { 0 };
        message.ToBytes(m_bytes);   // Translating object into bytes

        Crypt.Encrypt(m_bytes, Message::ByteSize());    // Encrypting bytes of message

        Radio.stopListening();  // Sending message
        Radio.write(m_bytes, Message::ByteSize());
        Radio.startListening();
    }

    Message Listen()
    {
        byte m_bytes[Message::ByteSize()] { 0 };
        byte pipe = 0;
        if (Radio.available(&pipe))
        {
            Radio.read(m_bytes, Message::ByteSize());
            Crypt.Decrypt(m_bytes, Message::ByteSize());
        }

        return Message::FromBytes(m_bytes);
    }
};


unsigned int RadioCommunicator::Message::QueryCounter = random(10);
