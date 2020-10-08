/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
#include <stdio.h>
#include <match.h>

static char ***result = NULL;
static int count = 0;


char **malloc_array(int n)
{
    char **array = (char **)malloc(sizeof(char *) * n);
        
    for (int j = 0; j < n; j++)
    {
        array[j] = (char *)malloc(sizeof(char) * (n + 1));
        memset(array[j], 0 , sizeof(char) * (n + 1));
    }

    return array;
}

char **copy_array(char **from, char **to, int n)
{
    if (!from || !to)
        return NULL;
    
    for(int i = 0; i < n; i++)
    {
        memcpy(to[i], from[i], n * sizeof(char));
    }

    return to;
}


void free_array(char **array, int n)
{
    for(int i = 0; i < n; i++)
    {
        free(array[i]);
    }

    free(array);
}

void set_line(char **array, int line, int column, int max, int num)
{
    if (num < 0)
        array[line][column] = 'Q';
    else
        array[line][column] = 0;

    //设置行
    for (int j = 0; j < max; j++)
    {
        if (j == column)
            continue;
        
        array[line][j] += num;
    }

    for (int i = 0; i < max; i++)
    {
        if (line == i)
            continue;
        
        //设置列
        array[i][column] += num;

        //设置斜线
        if (column - abs(line-i) >= 0)
            array[i][column - abs(line-i)] += num;

        if (column + abs(line-i) < max)
            array[i][column + abs(line-i)] += num;
    }
}

void print_arry(char **array, int max)
{
    for (int k = 0; k < max; k++)
    {
        for(int m = 0; m < max; m++)
        {
            //printf("array[%d][%d] = %d\n", k, m, array[k][m]);
            printf("%03d ", array[k][m]);
        }
        printf("\n");
    }
}
int queens(char **array, int line,/* int colum,*/ int max)
{
    if (line == max)
    {
        //成功生成一个解。这时返回查找失败，是为了让递归能继续进行，直到找出所有的解为止，这样不会有遗漏
        char **tmp = malloc_array(max); 
        copy_array(array, tmp, max);
        for (int i = 0; i < max; i++)
        {
            for(int j = 0; j < max; j++)
            {
                if (tmp[i][j] <= 0)
                    tmp[i][j] = '.';
            }
        }

        result[count] = tmp;
        count++;
        //printf("find count = %d\n", count);
        //print_arry(array, max);
        return -1;
    }

    int flag = 0;

    int ret = -1;
    //找到该行可以用的点
    for (int i = 0; i < max; i++)
    {
        if (array[line][i] == 0)
        {
            flag = 1;

            //设置同行同列、斜线为不可用。若当前点不是解，需要回退操作，同时还不影响原有的设置，那么如何回退？？
            //办法：使用+1和-1的办法实现。若值为0，表示可用，为负数表示不可用
            set_line(array, line, i, max, -1); 

            //printf("after set line = %d, column = %d\n", line, i);
            //print_arry(array, max);
            //递归下一行，若递归没有找到解，则需回退这一次的操作
            ret = queens(array, line + 1, max);
            if (ret != 0)
            {
                //printf("line %d need backup\n", line);
                set_line(array, line, i, max, 1);  //回退操作
            }
        }
    }

    //该行没有可用点，即无效解
    if (!flag)
    {
        //printf("line %d no vaild find\n", line);
        return -1;
    }
    
    /*if (ret < 0)
        printf("line %d fail\n", line);
    else
        printf("line %d ok\n", line);*/
    return ret;
}


 //n皇后最多有多少解呢？一定小于n吗？
//格数对应的解个数：
//1-1, 2-0, 3-0, 4-2, 5-10, 6-4, 7-40, 8-92, 9-352, 10-724, 11-2680
char *** solveNQueens(int n, int* returnSize, int** returnColumnSizes){
    //每次尝试生成一组解，生成成功则加入到结果中；
    //每填充一个Q，则将对应的行、列和斜线标记为不可填充(用'.'填充），只要值为'.'则不可填充Q
    int max = n * n * n;// * n;
    result = (char ***)malloc(sizeof(char **) * max);
    int *colum = (int *)malloc(sizeof(int) * max);
    char **array = malloc_array(n);

    count = 0;
    *returnSize = 0;
    if (!result || !colum || !array)
    {
        returnColumnSizes = NULL;
        return NULL;
    }

    for (int i = 0; i < max; i++)
    {
        memset(result, 0, (sizeof(char **) * max));
        memset(colum, 0, (sizeof(int) * max));
    }

    for (int i = 0; i < n; i++)
    {
        //在同一行同一个位置，可能存在多个解的情况。。。。。。但该代码设计只允许有一个解
        set_line(array, 0, i, n, -1);  //设置第一行
        //printf("after set line 0 column %d\n", i);
        //print_arry(array, n);
        int ret = queens(array, 1, n);   //寻找下一行的解
        if (ret != 0)
        {
            set_line(array, 0, i, n, 1); 
            continue;
        }
            
    }
    
    //printf("func = %s, line = %d\n", __func__, __LINE__);
    for (int i = 0; i < count; i++)
    {
        colum[i] = n;
    }

    *returnSize = count;
    *returnColumnSizes = colum;

//printf("func = %s, line = %d, count = %d, returnSize = %d\n", __func__, __LINE__, count, *returnSize);
    free_array(array, n);
    
    return result;
}



#if 0
/*该解法是递归return一次就记录一次。存在的问题在于，生成一次解后就return，那么下一次就会从新的位置找新的解，就会遗漏一些解。
改进的做法是每次生成一个解后，让递归不要return，而是直接放入到结果中，然后返回没有找到解，那么递归就可以一直进行下去，直到找出所有的解*/
void set_line(char **array, int line, int column, int max, int num)
{
    if (num < 0)
        array[line][column] = 'Q';
    else
        array[line][column] = 0;

    //设置行
    for (int j = 0; j < max; j++)
    {
        if (j == column)
            continue;
        
        array[line][j] += num;
    }

    for (int i = 0; i < max; i++)
    {
        if (line == i)
            continue;
        
        //设置列
        array[i][column] += num;

        //设置斜线
        if (column - abs(line-i) >= 0)
            array[i][column - abs(line-i)] += num;

        if (column + abs(line-i) < max)
            array[i][column + abs(line-i)] += num;
    }
}

void print_arry(char **array, int max)
{
    for (int k = 0; k < max; k++)
    {
        for(int m = 0; m < max; m++)
        {
            //printf("array[%d][%d] = %d\n", k, m, array[k][m]);
            printf("%c", array[k][m]);
        }
        printf("\n");
    }
}

int queens(char **array, int line,/* int colum,*/ int max)
{
    if (line == max)
    {
        return 0;
    }

    int flag = 0;

    int ret = -1;
    //找到该行可以用的点
    for (int i = 0; i < max; i++)
    {
        if (array[line][i] == 0)
        {
            flag = 1;

            //设置同行同列、斜线为不可用。若当前点不是解，需要回退操作，同时还不影响原有的设置，那么如何回退？？
            //办法：使用+1和-1的办法实现。若值为0，表示可用，为负数表示不可用
            set_line(array, line, i, max, -1); 

            //printf("after set line = %d, column = %d\n", line, i);
            //print_arry(array, max);
            //递归下一行，若递归没有找到解，则需回退这一次的操作
            ret = queens(array, line + 1, max);
            if (ret != 0)
            {
                //printf("line %d need backup\n", line);
                set_line(array, line, i, max, 1);  //回退操作
            }
        }
    }

    //该行没有可用点，即无效解
    if (!flag)
    {
        //printf("line %d no vaild find\n", line);
        return -1;
    }
    
    /*if (ret < 0)
        printf("line %d fail\n", line);
    else
        printf("line %d ok\n", line);*/
    return ret;
}

 //n皇后最多有多少解呢？一定小于n吗？
char *** solveNQueens(int n, int* returnSize, int** returnColumnSizes){
    //每次尝试生成一组解，生成成功则加入到结果中；
    //每填充一个Q，则将对应的行、列和斜线标记为不可填充(用'.'填充），只要值为'.'则不可填充Q

    char ***res = (char ***)malloc(sizeof(char **) * n);
    int *colum = (int *)malloc(sizeof(int) * n);
    char **array = NULL;

    *returnSize = 0;

    for (int i = 0; i < n; i++)
    {
        if (!array)
            array = (char **)malloc(sizeof(char *) * n);
        
        for (int j = 0; j < n; j++)
        {
            array[j] = (char *)malloc(sizeof(char) * (n + 1));
            memset(array[j], 0 , sizeof(char) * (n + 1));
        }

        /*for (int k = 0; k < n; k++)
        {
            for(int m = 0; m < n; m++)
            {
                array[k][m] = 'Q';
            }
        }*/
        set_line(array, 0, i, n, -1);  //设置第一行
        //printf("after set line 0 column %d\n", i);
        //print_arry(array, n);
        int ret = queens(array, 1, n);   //寻找下一行的解
        if (ret != 0)
        {
            set_line(array, 0, i, n, 1); 
            continue;
        }
            
        
        for (int k = 0; k < n; k++)
        {
            for(int m = 0; m < n; m++)
            {
                if (array[k][m] <= 0)
                    array[k][m] = '.';
            }
        }
        
        res[*returnSize] = array;
        colum[*returnSize] = n;
        (*returnSize)++;

        array = NULL;
    }

    //*returnSize = 0;
    *returnColumnSizes = colum;
    
    printf("*returnSize = %d\n", *returnSize);
	for (int i = 0 ; i < *returnSize; i++)
	{
		print_arry(res[i], 4);
        printf("\n");
	}
    return res;
}
#endif