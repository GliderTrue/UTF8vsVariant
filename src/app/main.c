#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "variant/variant.h"

#define SIZE_SP 1000000

int piB = 0;

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

int writeData(char *pathNC, char *pathV, uint32_t *spNum)
{
    FILE *fileNotCompr = NULL;
    FILE *fileVariant = NULL;
    fileNotCompr = fopen(pathNC, "wb");
    fileVariant = fopen(pathV, "wb");
    if (fileNotCompr == NULL || fileVariant == NULL)
    {
        printf("Error opening\n");
        return 0;
    }
    for (int i = 0; i != SIZE_SP; ++i)
    {
        uint32_t num = generate_number();
        spNum[i] = num;
        uint8_t *byte = (uint8_t *)malloc(sizeof(uint8_t) * 16);
        uint8_t *byteBuf = byte;
        size_t sizeVariant = encode_varint(num, &byte);
        piB += sizeVariant;
        fwrite(&num, sizeof(uint32_t), 1, fileNotCompr);
        fwrite(&byte, sizeof(uint8_t), sizeVariant, fileVariant);
        free(byteBuf);
    }
    fclose(fileNotCompr);
    fclose(fileVariant);
    return 1;
}

int readDataNotCompr(char *pathFile, uint32_t *spNum)
{
    FILE *fileNotCompr = NULL;

    fileNotCompr = fopen(pathFile, "rb");
    if (fileNotCompr == NULL)
    {
        printf("Error opening uncompressed");
        return 0;
    }
    for (int i = 0; i != SIZE_SP; ++i)
    {
        uint32_t num;
        fread(&num, sizeof(uint32_t), 1, fileNotCompr);
        if (num != spNum[i])
        {
            printf("error %d %d\n", num, spNum[i]);
            return;
        }
    }
    fclose(fileNotCompr);
    return 1;
}

int readDataVariant(char *pathFile, uint32_t *spNum)
{
    FILE *fileNotVariant = NULL;

    fileNotVariant = fopen(pathFile, "rb");
    if (fileNotVariant == NULL)
    {
        printf("Error opening uncompressed");
        return 0;
    }
    uint8_t *bufp = (uint8_t *)malloc(sizeof(uint8_t) * piB);
    uint8_t *copyBuf = bufp;
    fread(bufp, sizeof(uint8_t), piB, fileNotVariant);
    for (int i = 0; i != SIZE_SP; ++i)
    {
        uint32_t num = decode_varint(&bufp);
        if (num != spNum[i])
        {
            printf("error %d %d\n", num, spNum[i]);
            break;
        }
    }
    free(copyBuf);
    fclose(fileNotVariant);
    return 1;
}

int main()
{
    uint32_t *spNum = (uint32_t *)malloc(sizeof(uint32_t) * SIZE_SP);
    char *path1 = "resultData/notCompressed.dat";
    char *path2 = "resultData/variantCompressed.dat";
    writeData(path1, path2, spNum);
    readDataNotCompr(path1, spNum);
    readDataVariant(path2, spNum);
    return 0;
}
