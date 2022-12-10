#pragma once

#define ITERATE_KEYS

unsigned long long KEYS[12] = { 0x57838184D050A979LL, 0x17CC2403881B7822LL, 0xB22E5C1EA07A44ACLL, 
                                0xA5D4854A554DDF4FLL, 0xF4E5C7BB810D6A1FLL, 0xED50A979836872D7LL, 
                                0xC451D17063C8E091LL, 0x7571E6996BE8A31DLL, 0x484C2A06C68F4585LL, 
                                0x434B9D737BE7BB95LL, 0x1F1B313A29AC0A10LL, 0x2A9267EEDD760E44LL };

class Cryptographer
{
private:
    unsigned CurrentKey = 0;
    unsigned NextKey() { return (CurrentKey + 1) % 12; }
    bool ProbingNextKey = false;

    bool ImmitateWrongCryptoKey = false;

    unsigned long long GenerateCurrentCryptoKey() { return KEYS[ProbingNextKey ? NextKey() : CurrentKey]; }

public:
    Cryptographer() { }
    ~Cryptographer() { }

    void ProbeNextKey() { ProbingNextKey = true; Serial.print("Probing next key: "); PrintCurrentKey(); }
    void DeclineNextKey() { ProbingNextKey = false; Serial.print("Next key declined, returned: "); PrintCurrentKey(); }
    void AcceptNextKey() { CurrentKey = NextKey(); ProbingNextKey = false; Serial.print("Next key accepted: "); PrintCurrentKey(); }
    void SetNextKey() { CurrentKey = NextKey(); ProbingNextKey = false; Serial.print("Next key setted: "); PrintCurrentKey(); }

    void Encrypt(byte* bytes_to_encrypt, int length)
    {
        unsigned long long key = GenerateCurrentCryptoKey();
        byte* key_bytes = (byte*) &key;
        for (int i = 0, key_i = 0; i < length; i++, key_i = (key_i + 1) % sizeof(key))
            bytes_to_encrypt[i] += key_bytes[key_i];
    }

    void Decrypt(byte* bytes_to_decrypt, int length)
    {
        unsigned long long key = GenerateCurrentCryptoKey();
        byte* key_bytes = (byte*) &key;
        for (int i = 0, key_i = 0; i < length; i++, key_i = (key_i + 1) % sizeof(key))
            bytes_to_decrypt[i] -= key_bytes[key_i];
    }

    void PrintCurrentKey()
    {
        unsigned long long key = GenerateCurrentCryptoKey();
        PrintKey(key);
    }

    unsigned long long GetHash(unsigned long long data)
    {
        unsigned long long key = GenerateCurrentCryptoKey();
        unsigned long long hash = 0;
        byte* key_bytes = (byte*) &key;
        byte* hash_bytes = (byte*) &hash;
        byte* data_bytes = (byte*) &data;
        for (int i = 0; i < sizeof(data); i++)
            hash_bytes[i] = (key_bytes[i] & (data_bytes[i] << 1)) ^
                            ((key_bytes[i] << 1) & data_bytes[i]);
        return hash;
    }

    static void PrintKey(unsigned long long key)
    {
        byte* key_bytes = (byte*) &key;
        for (int i = sizeof(unsigned long long) - 1; i >= 0 ; i--)
        {
            int first_num = (key_bytes[i] >> 4) & 0x0F;
            int second_num = (key_bytes[i]) & 0x0F;
            char first_sym = first_num >= 10 ? 'A' + first_num - 10 : '0' + first_num;
            char second_sym = second_num >= 10 ? 'A' + second_num - 10 : '0' + second_num;
            Serial.print(first_sym); Serial.print(second_sym);
        }
        
        Serial.println(' ');
    }
};
