#include "huffman.h"

// 1.读取文本文件 + 统计字符频率
int ReadFileAndCount(char *filename)
{
    memset(freq, 0, sizeof(freq));
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("文件打开失败\n");
        exit(0);
    }
    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        freq[(unsigned char)ch]++;
    }
    fclose(fp);

    // 统计有效字符个数
    int n = 0;
    for (int i = 0; i < 258; i++)
        if (freq[i] > 0)
            n++;
    return n;
}

// 2.文件压缩：文本 → 二进制压缩文件
void CompressFile(char *srcFile, char *zipFile, HuffCode hc[], int n)
{
    FILE *fpIn = fopen(srcFile, "r");
    FILE *fpOut = fopen(zipFile, "wb");

    // 先把频率表写入压缩文件头部(解压必备)
    fwrite(freq, sizeof(int), 256, fpOut);

    char ch;
    char bitBuf[10000] = "";

    // 拼接所有哈夫曼编码
    while ((ch = fgetc(fpIn)) != EOF)
    {
        int idx = 0;
        for (int i = 0; i < 256; i++)
        {
            if (freq[i] > 0)
            {
                idx++;
                if (i == (unsigned char)ch)
                {
                    strcat(bitBuf, hc[idx].code);
                    break;
                }
            }
        }
    }

    // 先写入真实比特长度(解压必备)
    int len = strlen(bitBuf);
    fwrite(&len, sizeof(int), 1, fpOut);

    // 按8位一组转二进制写入文件
    unsigned char byte = 0;
    int cnt = 0;
    for (int i = 0; i < len; i++)
    {
        byte <<= 1;
        if (bitBuf[i] == '1')
            byte |= 1;
        cnt++;
        if (cnt == 8)
        {
            fputc(byte, fpOut);
            byte = 0;
            cnt = 0;
        }
    }
    // 处理剩余不足8位
    if (cnt > 0)
    {
        byte <<= (8 - cnt);
        fputc(byte, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
    printf("✅ 压缩完成，已生成二进制压缩文件\n");
}

// 3.解压：二进制压缩文件 → 还原文本文件
void UnZipFile(char *zipFile, char *outFile, HuffNode ht[], int n)
{
    FILE *fpIn = fopen(zipFile, "rb");
    FILE *fpOut = fopen(outFile, "w");

    int len;
    // 先读取头部频率表
    fread(freq, sizeof(int), 256, fpIn);
    // 在读取真实编码个数
    fread(&len, sizeof(int), 1, fpIn);

    int root = 2 * n - 1;
    int p = root;
    unsigned char ch;

    int count = 0;
    while (fread(&ch, 1, 1, fpIn) > 0)
    {
        // 逐位解析01
        for (int i = 7; i >= 0; i--)
        {
            count++;

            if (count >= len + 1)
            {
                fclose(fpIn);
                fclose(fpOut);
                printf("✅ 解压完成，已生成还原文本文件\n");
                return;
            }
            int bit = (ch >> i) & 1;
            if (bit == 0)
                p = ht[p].lch;
            else
                p = ht[p].rch;

            // 到达叶子节点，取出字符
            if (ht[p].lch == 0 && ht[p].rch == 0)
            {
                int idx = 0;
                for (int j = 0; j < 256; j++)
                {
                    if (freq[j] > 0)
                    {
                        idx++;
                        if (idx == p)
                        {
                            fputc(j, fpOut);
                            break;
                        }
                    }
                }
                p = root;
            }
        }
    }
    fclose(fpIn);
    fclose(fpOut);
    printf("✅ 解压完成，已生成还原文本文件\n");
}