template <typename T>
T Random()
{
    T result = 0;
    byte* result_bytes = (byte*) &result;
    for (int i = 0; i < sizeof(T); i++)
        result_bytes[i] = random(255);

    return result;
}
