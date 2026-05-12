#include "huffman.h"

int freq[256] = {0};

int main()
{
    int choice;
    char continueFlag;
    char inputPath[256];  // 用户输入：原文件 / 压缩文件
    char outputPath[256]; // 用户输入：压缩后 / 解压后文件

    // 主循环：一直运行，直到选择退出
    while (1)
    {
        // Linux 清屏
        system("clear");

        // ================= 菜单界面 =================
        printf("====== 哈夫曼压缩解压系统 ======\n");
        printf("1. 压缩文件\n");
        printf("2. 解压文件\n");
        printf("3. 查看当前文件夹所有文件（方便选择）\n");
        printf("0. 退出程序\n");
        printf("===============================\n");
        printf("请输入操作序号：");
        scanf("%d", &choice);

        while (getchar() != '\n');
        // 吸收回车，避免路径读取出错

        // ============== 0 → 退出程序 ==============
        if (choice == 0)
        {
            printf("程序已退出！\n");
            break;
        }

        // ============== 3 → 显示文件夹文件（Linux 命令）==============
        if (choice == 3)
        {
            printf("\n===== 当前文件夹文件列表 =====\n");
            system("ls -l");
            printf("\n按回车继续...\n");

            // 关键：清空输入缓冲区所有残留字符
            while (getchar() != '\n')
                ;
            continue;
        }

        // ============== 1 → 压缩文件 ==============
        if (choice == 1)
        {
            printf("\n===== 文件压缩 =====\n");
            printf("请输入**待压缩文件**路径：");
            gets(inputPath);
            printf("请输入**压缩后保存**路径：");
            gets(outputPath);

            // 1. 读文件统计频率
            int n = ReadFileAndCount(inputPath);
            if (n <= 0)
            {
                printf("文件打开失败！\n");
                goto END;
            }

            // 2. 初始化哈夫曼树
            HuffNode ht[MAX * 2] = {0};
            HuffCode hc[MAX] = {0};
            int pos = 1;
            for (int i = 0; i < 256; i++)
            {
                if (freq[i] > 0)
                    ht[pos++].weight = freq[i];
            }

            // 3. 建树 + 生成编码
            CreateHuffTree(ht, n);
            CreateHuffCode(ht, hc, n);

            // 4. 执行压缩
            CompressFile(inputPath, outputPath, hc, n);
            printf("✅ 压缩成功！\n");
        }

        // ============== 2 → 解压文件 ==============
        else if (choice == 2)
        {
            printf("\n===== 文件解压 =====\n");
            printf("请输入**压缩文件**路径：");
            gets(inputPath);
            printf("请输入**解压后保存**路径：");
            gets(outputPath);

            // 打开压缩文件检查是否存在
            FILE *fpCheck = fopen(inputPath, "rb");
            if (!fpCheck)
            {
                printf("压缩文件不存在！\n");
                goto END;
            }
            fclose(fpCheck);

            // ======================
            // 【重要修正】
            // 解压读取顺序：先读 频率表 → 再读 len
            // 完全匹配你的压缩逻辑
            // ======================
            FILE *fp = fopen(inputPath, "rb");
            int len;
            int tempFreq[256] = {0};

            // 【这里顺序改对了！】
            fread(tempFreq, sizeof(int), 256, fp); // 先读频率表
            fread(&len, sizeof(int), 1, fp);       // 再读有效位数 len
            fclose(fp);

            // 复制到全局频率表
            for (int i = 0; i < 256; i++)
                freq[i] = tempFreq[i];

            // 统计有效字符个数 n
            int n = 0;
            for (int i = 0; i < 256; i++)
                if (freq[i] > 0)
                    n++;

            // 重建哈夫曼树
            HuffNode ht[MAX * 2] = {0};
            int pos = 1;
            for (int i = 0; i < 256; i++)
            {
                if (freq[i] > 0)
                    ht[pos++].weight = freq[i];
            }
            CreateHuffTree(ht, n);

            // 执行解压
            UnZipFile(inputPath, outputPath, ht, n);
            printf("✅ 解压成功！\n");
        }

        // ============== 输入错误 ==============
        else
        {
            printf("输入错误，请重新选择！\n");
        }

    // 一次操作结束，询问是否继续
    END:
        printf("\n是否继续操作？(Y/N)：");
        scanf(" %c", &continueFlag);
        getchar();

        if (continueFlag == 'N' || continueFlag == 'n')
        {
            printf("程序结束！\n");
            break;
        }
    }

    return 0;
}