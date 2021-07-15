#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Search(FILE *output, int **map, int **key, int keymapsize, int rowForMap, int columnForMap, int, int);
int Result(int **map, int **key, int keymapsize, int row, int column); //prototypes

int main(int argc, char **argv)
{
    char *mapMatrixSize = argv[1];
    int keyMatrixSize = atoi(argv[2]);
    FILE *mapMatrix = fopen(argv[3], "r"); //map matrix
    FILE *keyMatrixFile = fopen(argv[4], "r");
    FILE *output = fopen(argv[5], "w+");

    char *token;
    int rowForMap, columnForMap;

    token = strtok(mapMatrixSize, "x");
    rowForMap = atoi(token);
    token = strtok(NULL, "x"); //map matrix row and column parsing from x
    columnForMap = atoi(token);

    int i, j;

    //Some codes to create 2d array...

    int **twoDarray = (int **)malloc(rowForMap * sizeof(int *));

    for (i = 0; i < rowForMap; i++)
        *(twoDarray + i) = (int *)malloc(columnForMap * sizeof(int));

    char aLine[1000];

    int a = 0, b = 0; //a is row b column

    while (!feof(mapMatrix))
    {
        fgets(aLine, 1000, mapMatrix);
        b = 0;
        char *tokenForNumbers;
        tokenForNumbers = strtok(aLine, " "); //2D array

        while (tokenForNumbers != NULL)
        {
            twoDarray[a][b] = atoi(tokenForNumbers);
            tokenForNumbers = strtok(NULL, " ");
            b++;
        }
        a++;
    }

    //KEY map codes ...
    a = 0;
    b = 0;
    char Line[256];
    int **keyMatrix = (int **)malloc(sizeof(int *) * keyMatrixSize);

    for (i = 0; i < keyMatrixSize; i++)
        *(keyMatrix + i) = (int *)malloc(sizeof(int) * keyMatrixSize);

    while (!feof(keyMatrixFile))
    {
        fgets(Line, 1000, keyMatrixFile);
        b = 0;
        char *tokenForKeys;
        tokenForKeys = strtok(Line, " ");

        while (tokenForKeys != NULL)
        {
            keyMatrix[a][b] = atoi(tokenForKeys); //passing numbers to key matrix
            tokenForKeys = strtok(NULL, " ");
            b++;
        }
        a++;
    }

    Search(output, twoDarray, keyMatrix, keyMatrixSize, rowForMap, columnForMap, 0, 0); //Search function

    fclose(mapMatrix);
    fclose(keyMatrixFile);
    fclose(output); //closing files 

    for (i = 0; i < rowForMap; i++)
    {
        free(twoDarray[i]);
    }
    free(twoDarray);
    //free the map matrix and key matrix line by line
    for (i = 0; i < keyMatrixSize; i++)
    {
        free(keyMatrix[i]);
    }
    free(keyMatrix);
    return 0;
}

void Search(FILE *output, int **map, int **key, int keymapsize, int rowForMap, int columnForMap, int row, int column)
{

    //6. and 7. Parameter row and column where the map matrix start to multiply
    int result = 0;

    result = Result(map, key, keymapsize, row, column);

    fprintf(output, "%d,%d:%d", row + keymapsize / 2, column + keymapsize / 2, result); //row +keymapsize/2 and column+keymapsize/2 are CENTER of SUB-MATRIX

    switch (result % 5) //while case is in if statement you are in estimated direction
    {                   //while case is in else statement you are in reverse estimated direction
    case 0:
        return;
    case 1:
        fprintf(output, "\n");
        if (!(row - keymapsize < 0)) //while you can go to upward 
        {                            //upward
            row -= keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
        else
        { //downward because you can not go to upward
            row += keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
    case 2:
        fprintf(output, "\n");
        if (!(row + keymapsize >= rowForMap))
        { //downward
            row += keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
        else
        { //upward because there is no downward
            row -= keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
    case 3:
        fprintf(output, "\n");
        if (!(column + keymapsize >= columnForMap)) //if you are not gonna take out from right go
        {                                           //right
            column += keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
        else
        { //left because no right
            column -= keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
    case 4:
        fprintf(output, "\n");
        if (!(column - keymapsize < 0))
        { //left
            column -= keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
        else
        { //right because no left
            column += keymapsize;
            return Search(output, map, key, keymapsize, rowForMap, columnForMap, row, column);
        }
    }
    return;
}

int Result(int **map, int **key, int keymapsize, int row, int column)
{ //it multiplies same row-column pairs and returns the result
    //row and column where the map matrix starts to multiply
    int result = 0, i, j;
    for (i = 0; i < keymapsize; i++)
        for (j = 0; j < keymapsize; j++)
            result = result + (map[i + row][j + column] * key[i][j]);
    return result;
}