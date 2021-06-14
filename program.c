#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct LIST
{
    int count;
    int value;
    struct LIST *next;
} pixel;

int **allocate(int, int);
void readLinkedList(FILE *, pixel *, int pxCount);
int **readImage(FILE *, int, int);
void insertLast(pixel *, int, int);
char *removeString(char *, int);
pixel *head;
pixel *last;
int main(void)
{
    char filename[40];
    char *file;

    int i, j, count, flag = 0;
    int width, height;
    char chunk[30];
    printf("\n\nRLE File Encoder - Decoder\t|\n\t\t\t\t| Umutcan Sevdi\n\t\t\t\t| 19011091 \nCommand List\n\t%ccompress  :\tCompresses an image with RLE, saves as '.sqz'.\n\t%cconstruct :\tDisplays the avaliable commands for the encoded file.\n> ", 192, 192);
    char command[5];
    scanf("%s", command);
    if (strcmp(command, "compress") == 0)
    {
        printf("\n>Compress Image");

        printf("\n%c Enter the file name without the extension\n> ", 192);
        scanf("%s", &filename);

        int **matrix;

        strcat(filename, ".pgm");
        file = removeString(filename, 4);

        FILE *fp = fopen(filename, "r");

        if (fp == NULL)
        {
            perror(">Error\t| Unable to open file");
            exit(1);
        }
        else
        {
            printf("File\t| %s |", filename);
        }
        rewind(fp);
        fscanf(fp, "%s", &chunk);

        fscanf(fp, "%d %d", &width, &height);
        fscanf(fp, "%s", &chunk);
        matrix = readImage(fp, height, width);
        fclose(fp);

        pixel *current = (pixel *)malloc(sizeof(pixel));
        head = current;
        last = current;
        current->next = NULL;
        printf("\nStats\t| Width : %d \t |\tHeight : %d\n", width, height);
        for (i = 0; i < height; i++)
        {
            printf("%c Read Line %d\n", 192, i);
            for (j = 0; j < width; j++)
            {
                if (j + 1 < width)
                {
                    if (matrix[i][j] == matrix[i][j + 1])
                        count++;
                    else
                    {
                        insertLast(current, matrix[i][j], count);
                        count = 0;
                    }
                }
                else if (i + 1 < height)
                {
                    if (matrix[i][j] == matrix[i + 1][0])
                        count++;
                    else
                    {
                        insertLast(current, matrix[i][j], count);
                        count = 0;
                    }
                }
                else
                {
                    insertLast(current, matrix[i][j], count);
                    count = 0;
                }
            }
        }
        printf("\n>Result\t| Reading Completed");
        fp = fopen(strcat(file, ".sqz"), "w");

        if (fp == NULL)
        {
            printf("\n>Error\t| Unable to open file");
            exit(1);
        }
        else
        {
            printf("\n>File\t| %s ", file);
        }
        fprintf(fp, "%d %d\n", width, height);
        current = head;
        while (current->next != NULL)
        {
            fprintf(fp, "%d %d ", current->value, current->count);
            current = current->next;
        }
        printf("\n>Result\t| Writing Completed");
        fclose(fp);

        for (i = 0; i < height; i++)
            free(matrix[i]);
        free(matrix);
    }
    else if (strcmp(command, "construct") == 0)
    {
        char ch;
        printf("\n>Construct Menu");

        printf("\n%c Enter the file name without the extension\n> ", 192);
        scanf("%s", &filename);
        strcat(filename, ".sqz");
        file = removeString(filename, 4);
        FILE *fp = fopen(filename, "r");

        if (fp == NULL)
        {
            perror(">Error\t| Unable to open file");
            exit(1);
        }
        else
        {
            printf("File\t| %s |", filename);
        }
        rewind(fp);
        fscanf(fp, "%d %d", &width, &height);

        pixel *current = (pixel *)malloc(sizeof(pixel));
        head = current;
        last = current;
        current->next = NULL;
        printf("\nStats\t| Width : %d \t |\tHeight : %d\n", width, height);
        printf("\n>Reading Elements...");

        readLinkedList(fp, current, width * height);
        fclose(fp);
        printf("\n>Result\t| Reading Completed");

        int max = 0;
        current = head;
        last = current;
        i = 16;
        while (current->next != NULL)
        {
            if (current->value > 256 || current->value < 0)
            {
                printf("\n>Error\t| File color interval is out of bounds");
                exit(2);
            }
            else if (current->value == current->next->value)
            {
                printf("\n>Error\t| File is not encoded as expected");
                exit(3);
            }
            else if (current->value > max)
                max = current->value;

            if (i > 15)
            {
                i = 0;
                printf("\n%c Output %d ", 192, current->value);
            }
            else
            {
                i++;
                printf("%d ", current->value);
            }
            current = current->next;
        }
        printf("\nCommand List\n\t%cdecode    :\tReconstructs a compressed image, saves as '.pgm''.\n\t%crecolor   :\tChanges the all instances of a color in the file.\n\t%chistogram :\tDisplays the color scale in histogram.\n> ", 192, 192, 192);
        scanf("%s", command);
        if (strcmp(command, "decode") == 0)
        {
            printf("\nDecode");
            fp = fopen(strcat(file, "_decoded.pgm"), "w");

            if (fp == NULL)
            {
                printf("\n>Error\t| Unable to open file");
                exit(1);
            }
            else
            {
                printf("\n>File\t| %s ", file);
            }
            fprintf(fp, "P2\n");
            fprintf(fp, "%d %d\n", width, height);
            fprintf(fp, "%d\n", max);

            current = head;
            j = 0;
            while (current->next != NULL)
            {
                for (i = 0; i <= current->count; i++)
                {
                    if (j >= width)
                    {
                        j = 1;
                        fprintf(fp, "\n");
                    }
                    else
                        j++;
                    if (current->value > 9)
                        fprintf(fp, "%d ", current->value);
                    else
                        fprintf(fp, "%d  ", current->value);
                }

                current = current->next;
            }
            printf("\n>Result\t| Writing Completed");
            fclose(fp);
        }
        else if (strcmp(command, "recolor") == 0)
        {
            printf("\nEdit Color\n%cEnter the color you would like to change\n> ", 192);
            int oldColor;
            int newColor;
            scanf("%d", &oldColor);
            printf("\nEdit Color\n%cEnter the color you would like to replace with\n> ", 192);
            scanf("%d", &newColor);

            if (newColor == oldColor)
            {
                printf("\n>Error\t| Properties are equal");
                exit(1);
            }
            else
            {
                printf("\n\tOld Color : %d\n\tNew Color : %d", oldColor, newColor);
            }

            current = head; //Color Swap
            last = current;
            printf("\n>Color Change\n");
            while (current->next != NULL)
            {
                if (current->value == oldColor)
                {
                    current->value = newColor;
                    printf("%d%c%d[%d]  ", current->value, 175, oldColor, current->count);
                }
                else
                {
                    printf("%d[%d]  ", current->value, current->count);
                }

                current = current->next;
            }
            current = head; //Duplicate Remove
            last = current;
            pixel *tmpCurrent = (pixel *)malloc(sizeof(pixel));
            printf("\n");
            while (current->next != NULL)
            {
                printf("%d  ", current->value);
                flag = 1;
                tmpCurrent = current->next;
                while (tmpCurrent != NULL && flag)
                {
                    if (tmpCurrent->value == current->value)
                    {
                        printf("-> ");
                        current->count += tmpCurrent->count + 1;

                        tmpCurrent = tmpCurrent->next;
                    }
                    else
                    {
                        flag = 0;
                        current->next = tmpCurrent;
                        if (current->next->next == NULL)
                            printf(" | ");
                    }
                }

                current = current->next;
            }

            current = head;
            last = current;
            printf("\n>Final\n");
            j = 0;
            while (current->next != NULL)
            {
                printf("%d[%d]  ", current->value, current->count);
                j += current->count + 1;
                current = current->next;
            }
            printf("\n>Updating File\t|");
            fp = fopen(filename, "w");

            if (fp == NULL)
            {
                printf("\n>Error\t| Unable to open file");
                exit(1);
            }
            else
            {
                printf("\n>File\t| %s ", file);
            }
            fprintf(fp, "%d %d\n", width, height);
            current = head;
            last = current;
            while (current->next != NULL)
            {
                fprintf(fp, "%d %d ", current->value, current->count);
                current = current->next;
            }
            printf("\n>Result\t| Writing Completed");
            fclose(fp);
        }
        else if (strcmp(command, "histogram") == 0)
        {

            pixel *tmpHead;
            pixel *tmpCurrent = (pixel *)malloc(sizeof(pixel));

            tmpHead = tmpCurrent;
            tmpCurrent->value = 0;
            tmpCurrent->count = 0;
            tmpCurrent->next = NULL;

            printf("\nHistogram");
            current = head;
            last = current;
            j = 0;
            while (current->next != NULL)
            {
                flag = 0;
                tmpCurrent = tmpHead;
                while (tmpCurrent->next != NULL && flag == 0)
                {
                    if (tmpCurrent->value == current->value)
                    {
                        tmpCurrent->count += current->count + 1;
                        flag = 1;
                    }
                    tmpCurrent = tmpCurrent->next;
                }
                if (flag == 0)
                {
                    tmpCurrent = tmpHead;
                    while (tmpCurrent->next != NULL)
                    {
                        tmpCurrent = tmpCurrent->next;
                    }
                    tmpCurrent->next = (pixel *)malloc(sizeof(pixel));
                    tmpCurrent = tmpCurrent->next;
                    tmpCurrent->value = current->value;
                    tmpCurrent->count = current->count + 1;
                    tmpCurrent->next = NULL;
                }
                j += current->count;
                current = current->next;
            }
            printf("\n\nHistogram Output : ");
            tmpCurrent = tmpHead->next;

            while (tmpCurrent->next != NULL)
            {
                printf("\n%d\t[%d/%d]\t", tmpCurrent->value, tmpCurrent->count, j);
                for (i = 0; i < (int)((float)(tmpCurrent->count) / j * 100); i++)
                {
                    printf("|");
                }
                tmpCurrent = tmpCurrent->next;
            }
        }
        else
        {
            printf("\nInvalid Command");
        }
    }
    else
    {
        printf("\nInvalid Command");
    }
}

int **readImage(FILE *fptr, int row, int col)
{

    int i, j;
    int **matrix = (int **)allocate(row, col); //=  malloc(sizeof(int*)*)
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            fscanf(fptr, " %d", &matrix[i][j]);
        }
    }
    return matrix;
}
void readLinkedList(FILE *fp, pixel *current, int pxCount)
{
    int i = 0;
    int value, count;
    while (fscanf(fp, "%d %d", &value, &count) != EOF)
    {
        insertLast(current, value, count);
        i += (count + 1);
    }
    if (i != pxCount)
    {
        printf("\n>Error\t| Encoded file's pixel count is not matching with the expected value");
        exit(3);
    }
}

int **allocate(int row, int col)
{
    int **mat = (int **)malloc(row * sizeof(int *));
    int i;
    for (i = 0; i < row; i++)
        mat[i] = (int *)malloc(col * sizeof(int));
    return mat;
}

void insertLast(pixel *current, int value, int count)
{
    last = current;
    current = head;
    if (last->next == NULL)
    {
        current->value = value;
        current->count = count;
        current->next = (pixel *)malloc(sizeof(pixel));
        current->next->next = NULL;
        last = current;
    }
    else
    {
        if (current->next == NULL)
        {
            current->value = value;
            current->count = count;
            current->next = (pixel *)malloc(sizeof(pixel));
            current->next->next = NULL;
        }
        else
        {
            while (current->next != NULL)
                current = current->next;
            current->next = (pixel *)malloc(sizeof(pixel));
            current->value = value;
            current->count = count;
            current->next->next = NULL;
        }
    }
}
char *removeString(char *string, int last)
{
    char *str = (char *)malloc(strlen(string) * sizeof(char));
    strcpy(str, string);
    if ((strlen(str) - last) > 0)
    {
        str[strlen(str) - last] = '\0';
    }
    return str;
}
