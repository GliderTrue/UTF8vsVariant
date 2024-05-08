#include <stdio.h>
#include <stdint.h>

uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90)
    {
        return r % 128;
    }
    if (p < 95)
    {
        return r % 16384;
    }
    if (p < 99)
    {
        return r % 2097152;
    }
    return r % 268435455;
}

int writeData(char *pathFile)
{
    FILE *file = NULL;

    file = fopen(pathFile, "wb");

    if (file == NULL)
    {
        printf("Error opening uncompressed");
        return 0;
    }

    for (int i = 0; i != 100; ++i)
    {
        uint32_t num = generate_number();
        fwrite(&num, sizeof(uint32_t), 1, file);
    }
    fclose(file);
    return 1;
}

int readData(char *pathFile)
{
    FILE *file = NULL;

    file = fopen(pathFile, "rb");
    if (file == NULL)
    {
        printf("Error opening uncompressed");
        return 0;
    }
    for (int i = 0; i != 100; ++i)
    {
        uint32_t num;
        fread(&num, sizeof(uint32_t), 1, file);
        printf("%d\n", num);
    }
    fclose(file);
    return 1;
}

int main()
{
    char *path = "resultData/notCompressed.dat";
    writeData(path);
    readData(path);
    return 0;
}
