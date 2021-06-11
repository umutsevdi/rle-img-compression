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
void readLinkedList(FILE *, pixel *);
int **readImage(FILE *, int, int);
void insertLast(pixel *, int, int);
char *removeString(char *, int);
pixel *head;
pixel *last;
int main(void)
{
    char filename[40];
    char *file;

    int i, j = 0, count = 0;
    int width, height;
    char chunk[30];
    printf("\n\nRLE File Encoder - Decoder\t|\n\t\t\t\t| Umutcan Sevdi\n\t\t\t\t| 19011091 \nCommand List\n\t%ccompress  :\tCompresses an image with RLE, saves as '.sqz'.\n\t%cconstruct :\tReconstructs a compressed image, saves as '.desqz'.\n\t%cdisplay   :\tDisplays '.sqz' and '.desqz' file formats as image on terminal.\n> ", 192, 192,192);
    char command[5];
    scanf("%s", command);
    if (strcmp(command, "compress") == 0)
    {
        printf("\n>Compress Image");

        printf("\n%c Enter the file name without the extension\n> ", 192);
        scanf("%s", &filename);

        int **matrix;

        strcat(filename, ".ascii.pgm");
        file = removeString(filename, 10);

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
        fscanf(fp, "%s", &chunk);
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
        printf("\n>Construct Image");

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
        printf("\n>Read Elements");
        readLinkedList(fp, current);
        fclose(fp);
        printf("\n>Result\t| Reading Completed");
        fp = fopen(strcat(file, ".desqz"), "w");

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
        fprintf(fp, "# %s\n", file);
        fprintf(fp, "%d %d\n", width, height);
        int max = 0;
        current = head;
        while (current->next != NULL)
        {
            if (current->value > max)
                max = current->value;
            current = current->next;
        }
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
void readLinkedList(FILE *fp, pixel *current)
{

    int value, count;
    int counter = 10;
    while (fscanf(fp, "%d %d", &value, &count) != EOF)
    {
        insertLast(current, value, count);
        if (counter < 10)
        {
            printf("%d,", value);
            counter++;
        }
        else
        {
            printf("\n%c Read\t| %d,", 192, value);
            counter = 0;
        }
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
