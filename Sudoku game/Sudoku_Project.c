#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define threads_number 9

pthread_mutex_t mut1;


int check_duplicate_arr[threads_number+2] = {0};
int invalid_count = 0;
int incol_i_r[9] = {0};
int incol_i_c[9] = {0};
int incol_v[9] = {0};
int inrow_i_r[9] = {0};
int inrow_i_c[9] = {0};
int inrow_v[9] = {0};
int in3x3_i_r[9] = {0};
int in3x3_i_c[9] = {0};
int in3x3_v[9] = {0};
int moves = 0;

typedef struct 
{
	int row;
	int column;		
} validity_parameters;

typedef struct 
{
	int row;
	int column;
    int value;		
} solution_parameters;

int sudoku_9x9_grid[9][9] = {
	{4, 2, 6, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {6, 9, 4, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

void *Column_Validation(void *args)//function to validate each column. It iterates through each column and returns true if all columns are resolved and don't contain duplicate entries
{
    pthread_mutex_lock(&mut1);//synchronization technique used
    int chk = 1;
    int valid_arr[10] = {0};
    int j = 0;
    int num_check = 0;
    int k = 0;
    for(int i = 0; i < 9 ; i++)//row 
    {
        for (int loop = 0; loop < 10; loop++)
        {
            valid_arr[loop] = 0;
        }
        
        for(j = 0 ; j < 9 ; j++)//col
        {
            num_check = sudoku_9x9_grid[j][i];
            if(num_check < 1 || num_check > 9)
            {
                printf("Column or Row entries = %d ,are out of bound!\n",num_check);
                pthread_exit(NULL);
            }
            else if(valid_arr[num_check] == 1)
            {
                incol_i_c[k] = i;//saving column index of wrong entry
                incol_i_r[k] = j;//saving row index of wrong entry
                incol_v[k] = num_check;//saving value of wrong entry
                printf("Number = %d, has duplicate on Index = [%d][%d]\n",incol_v[k],incol_i_r[k]+1,incol_i_c[k]+1);
                k++;
                chk = 2;
                invalid_count++;//increasing invalid entry count
            }
            else
            {
                valid_arr[num_check] = 1;
            }
        }
        
    }
   if(chk == 1)
   check_duplicate_arr[0] = 1;//1 in validityarray indicates that all columns are valid
   if(chk == 2)
   check_duplicate_arr[0] = 0;
   pthread_mutex_unlock(&mut1);
   return (void*)chk;
   pthread_exit(NULL);
}

void  *Row_Validation(void *args)//function to validate each row. It iterates through each row and returns true if all rows are resolved and don't contain duplicate entries
{
        pthread_mutex_lock(&mut1);//synchronization technique used
        int chk = 1;
        int valid_arr[10] = {0};
        int j = 0;
        int k = 0;

        for (int i = 0; i < 9; i++) // row
        {
            for (int loop = 0; loop < 10; loop++)
            {
            valid_arr[loop] = 0;
            }   

            
            for (j = 0 ; j < 9 ; j++) // col
            {
                int check_num = sudoku_9x9_grid[i][j];
                if(check_num < 1 || check_num > 9)
                {
                    printf("Column and Row entries are out of bound!\n");
                    pthread_exit(NULL);
                }
                else if(valid_arr[check_num] == 1)
                {
                    inrow_i_r[k] = i;//saving row index of wrong entry
                    inrow_i_c[k] = j;//saving column index of wrong entry
                    inrow_v[k] = check_num;//saving value of wrong entry
                    printf("Number = %d, has duplicate on Index = [%d][%d]\n",inrow_v[k],inrow_i_r[k]+1,inrow_i_c[k]+1);
                    k++;
                    chk = 2;
                    invalid_count++;//increasing invalid entry count
                }
                else
                {
                    valid_arr[check_num] = 1;
                }
            }
        }
        if(chk == 1)
        check_duplicate_arr[1] = 1;//1 in validityarray indicates that all rows are valid
        if(chk == 2)
        check_duplicate_arr[1] = 0;
        pthread_mutex_unlock(&mut1);
        return (void*)chk;
        pthread_exit(NULL);
}

void *three_Grid_Validation(void *args)
{
    pthread_mutex_lock(&mut1);//synchronization technique used
    int k = 0;
    int chk = 1;
    int valid_arr[10] = {0};
    int value = 0;
    validity_parameters * para = (validity_parameters*) args;
    int r = para->row;
    int c = para->column;
    if(c > 6 || r > 6 || c % 3 != 0 || r % 3 != 0)
    {
       printf("Given entries for Row and Columns are out of bound!\n");
       pthread_exit(NULL);
    }
    for (int i = r ; i < r + 3 ; i++)
    {

        for (int loop = 0; loop < 10; loop++)
        {
            valid_arr[loop] = 0;
        }   

        for (int j = c ; j < c + 3 ; j++)
        {
            value = sudoku_9x9_grid[i][j];
            if(value < 1 || value > 9)
            {
                printf("The values in sudoku table are out of bound!\n");
            }
            else if(valid_arr[value] == 1)
            {
                in3x3_i_r[k] = i;//saving row index of wrong entry
                in3x3_i_c[k] = j;//saving column index of wrong entry
                in3x3_v[k] = value;//saving value of wrong entry
                printf("Number = %d ,has duplicate on Index = [%d][%d]\n",in3x3_v[k],in3x3_i_r[k]+1,in3x3_i_c[k]+1);
                chk = 2;
                k++;
                invalid_count++;//increasing invalid entry count
            }
            else
            {
                valid_arr[value] = 1;
            }
        }
    }
    if (chk == 1)
    check_duplicate_arr[r + c/3] = 1;//1 in validityarray indicates that this 3x3 grid is valid
    pthread_mutex_unlock(&mut1);
    return (void*)chk;
    pthread_exit(NULL);
    
}

void *suduko_rowsolver(void *args)
{
    pthread_mutex_lock(&mut1);//synchronization technique used
    solution_parameters * para = (solution_parameters*) args;
    pthread_t t1,t2;
    int rowind = para->row;
    int colind = para->column;
    int val = para->value;
    int findreplaceentry[10] = {0};
    int replacemententry;

    for(int i = rowind,j = 0; j < 9; j++)
    {
        findreplaceentry[sudoku_9x9_grid[i][j]] = 1;//1 at a specific index of findreplaceentry indicates that this value is found in row
    }
    for(int i = 0; i <= 9; i++)
    {
        if(findreplaceentry[i] == 0 && i != 0)
            replacemententry = i;
    }

    printf("Entry to be placed = %d\n",replacemententry);
    
    if(replacemententry >= 1 && replacemententry <= 9)
    {
        sudoku_9x9_grid[rowind][colind] = replacemententry;//swaping the row entry in sudoku 2D matrix
        moves++;
    }
    pthread_mutex_unlock(&mut1);
    pthread_exit(NULL);
}

void *suduko_colsolver(void *args)
{
    pthread_mutex_lock(&mut1);//synchronization technique used
    solution_parameters * para = (solution_parameters*) args;
    pthread_t t1,t2;
    int rowind = para->row;
    int colind = para->column;
    int val = para->value;
    int findreplaceentry[10] = {0};
    int replacemententry;

    for(int i = colind,j = 0; j < 9; j++)
    {
        findreplaceentry[sudoku_9x9_grid[j][i]] = 1;//1 at a specific index of findreplaceentry indicates that this value is found in row
    }
    printf("\n");
    for(int i = 0; i <= 9; i++)
    {
        if(findreplaceentry[i] == 0 && i != 0)
            replacemententry = i;
    }

    if(replacemententry >= 1 && replacemententry <= 9)
    {
        printf("Entry to be placed = %d\n",replacemententry);
        sudoku_9x9_grid[rowind][colind] = replacemententry;//swaping the column entry in sudoku 2D matrix
        moves++;
    }
    pthread_mutex_unlock(&mut1);
    pthread_exit(NULL);
}

void *suduko_3x3solver(void *args)
{
    pthread_mutex_lock(&mut1);//synchronization technique used
    solution_parameters * para = (solution_parameters*) args;
    pthread_t t1,t2;
    int rowind = para->row;
    int colind = para->column;
    int val = para->value;
    int findreplaceentry[10] = {0};
    int replacemententry;
    int faultyrow,faultycol;

    for(int i = rowind; i < rowind+3; i++)
    {
        for(int j = colind; j < colind+3; j++)
        {
            if(findreplaceentry[sudoku_9x9_grid[i][j]] == 1)//If 1 at a specific index already than it indicates that duplication has occured
            {
                faultyrow = i;
                faultycol = j;
            }
            else
            {
                findreplaceentry[sudoku_9x9_grid[i][j]] = 1;//1 at a specific index of findreplaceentry indicates that this value is found in row
            }
        }
    }
    printf("\n");
    for(int i = 0; i <= 9; i++)
    {
        if(findreplaceentry[i] == 0 && i != 0)
            replacemententry = i;
    }
    if(replacemententry >= 1 && replacemententry <= 9)
    {
        printf("Entry to be placed = %d\n",replacemententry);
        sudoku_9x9_grid[faultyrow][faultycol] = replacemententry;//swaping the entries of 3x3 grid in sudoku 2D matrix
        moves++;
    }
    pthread_mutex_unlock(&mut1);
    pthread_exit(NULL);
}

int main()
{
    void * status = 0;
    pthread_t t1,t2,threads[threads_number];
    pthread_mutex_init(&mut1,NULL);//initializing mutex tobe used later in functions
    int ind = 0;

    printf("Matrix before solution\n");//printing sudoku 9x9 grid
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ",sudoku_9x9_grid[i][j]);
        }
        printf("\n");
    }
 
    printf("\nColumn Validation\n");
    pthread_create(&t1,NULL,Column_Validation,NULL);//thread for column validation
    printf("Thread ID for column validation = %d\n",t1);
    pthread_join(t1,&status);
    
    if((int)status == 1)
    printf("\nThe value is = %d , Sudoku has no duplicate entries!\n",(int)status);
    else
    printf("\nThe value is = %d , Sudoku has duplicate entries!\n",(int)status);

    if(check_duplicate_arr[0] == 1)
    {
        printf("\nCancelling Column validation thread with thread id = %d\n",t1);
        pthread_cancel(t1);
    }
    
    printf("\nRow Validation\n");
    pthread_create(&t2,NULL,Row_Validation,NULL);//thread for row validation
    printf("Thread ID for row validation = %d\n",t2);
    pthread_join(t2,&status);

    if((int)status == 1)
    printf("\nThe value is = %d , Sudoku has no duplicate entries!\n",(int)status);
    else
    printf("\nThe value is = %d , Sudoku has duplicate entries!\n",(int)status);
    
    if(check_duplicate_arr[1] == 1)
    {
        printf("\nCancelling row validation thread with thread id = %d\n",t2);
        pthread_cancel(t2);
    }

    printf("\n3x3 Validation\n");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if(i % 3 == 0 && j % 3 == 0)
            {
            validity_parameters *para = (validity_parameters*) malloc(sizeof(validity_parameters));
            para->row = i;
            para->column = j;
            pthread_create(&threads[ind],NULL,three_Grid_Validation,para);//threads for 3x3 grid validation
            printf("Thread ID for 3x3 validation = %d, Index = %d\n",threads[ind],ind);
            pthread_join(threads[ind], &status);
            if((int)status == 1)
            printf("\nThe value is = %d , Sudoku has no duplicate entries!\n",(int)status);
            else
            printf("\nThe value is = %d , Sudoku has duplicate entries!\n",(int)status);
            ind++;
            }
        }
        
    }

    while(1)//loop until all duplications are resolved
    {
        printf("\nTotal invalid entries = %d\n",invalid_count);
        
        int counter1 = 0,counter2 = 0,counter3 = 0;
        
        for (int  i = 0; i < 9; i++)
        {
            if (in3x3_v[i] != 0 || in3x3_v[i] != NULL)
            {
                counter3++;//counter for invalid entries in 3x3 grid
            }
            if (incol_v[i] != 0 || incol_v[i] != NULL)
            {
                counter2++;//counter for invalid entries in columns
            }
            if (inrow_v[i] != 0 || inrow_v[i] != NULL)
            {
                counter1++;//counter for invalid entries in rows
            }
        }
        
        printf("\nrow issues = %d\n",counter1);
        printf("col issues = %d\n",counter2);
        printf("3x3 issues = %d\n",counter3);
        pthread_t t3[counter1];//threads against invalid entries in rows
        pthread_t t4[counter2];//threads against invalid entries in columns
        pthread_t t5[counter3];//threads against invalid entries in 3x3 grids

        
        for (int  i = 0; i < counter1; i++)
        {
            solution_parameters *d = (solution_parameters*) malloc(sizeof(solution_parameters));
            d->value = inrow_v[i];
            d->row = inrow_i_r[i];
            d->column = inrow_i_c[i];
            printf("\nValue at trouble = %d, row = %d, column = %d\n",d->value,d->row+1,d->column+1);
            pthread_create(&t3[i],NULL,suduko_rowsolver,d);//thread calling fuction to resolve duplication in rows
            printf("Thread ID for row solution = %d\n",t3);
            pthread_join(t3[i],NULL);
        }
        for (int  i = 0; i < counter2; i++)
        {
            solution_parameters *d = (solution_parameters*) malloc(sizeof(solution_parameters));
            d->value = incol_v[i];
            d->row = incol_i_r[i];
            d->column = incol_i_c[i];
            printf("Value at trouble = %d, row = %d, column = %d\n",d->value,d->row+1,d->column+1);
            pthread_create(&t4[i],NULL,suduko_colsolver,d);//thread calling fuction to resolve duplication in columns
            printf("Thread ID for column solution = %d\n",t4);
            pthread_join(t4[i],NULL);
        }

        for (int  i = 0; i < counter3; i++)
        {
            solution_parameters *d = (solution_parameters*) malloc(sizeof(solution_parameters));
            for(int i = 0; i < 9; i++)
            {
                for(int j = 0; j < 9; j++)
                {
                    if(i%3 == 0 && j%3 == 0)
                    {
                        d->value = in3x3_v[i];
                        d->row = i;
                        d->column = j;
                        printf("Value at trouble = %d, row = %d, column = %d\n",d->value,d->row+1,d->column+1);
                        pthread_create(&t5[0],NULL,suduko_3x3solver,d);//thread calling fuction to resolve duplication in 3x3 grid
                        printf("Thread ID for 3x3 solution = %d, Index = %d\n",t5[0],0);
                        pthread_join(t5[0],NULL);
                    }
                }
            }
        }

        pthread_create(&t1,NULL,Row_Validation,NULL);//again row validation
        printf("Thread ID for column validation = %d\n",t1);
        pthread_join(t1,NULL);
        pthread_create(&t2,NULL,Column_Validation,NULL);//again column validation
        printf("Thread ID for row validation = %d\n",t2);
        pthread_join(t2,NULL);
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if(i % 3 == 0 && j % 3 == 0)
                {
                validity_parameters *para = (validity_parameters*) malloc(sizeof(validity_parameters));
                para->row = i;
                para->column = j;
                pthread_create(&threads[ind],NULL,three_Grid_Validation,para);//again 3x3 grid validation
                printf("Thread ID for 3x3 validation = %d, Index = %d\n",threads[ind],ind);
                pthread_join(threads[ind], NULL);
                ind++;
                }
            }
        }

        int va0 = check_duplicate_arr[0], va1 = check_duplicate_arr[1];

        if(va0 == 1 && va1 == 1)//breaks while loop if all rows and columns are valid & resolved
        {
            printf("\nSudoku has become valid!\n");
            break;
        }
    }

    printf("Number of moves = %d\n",moves);

    printf("Matrix after solution\n");//printing matrix after solution
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ",sudoku_9x9_grid[i][j]);
        }
        printf("\n");
    }

    pthread_mutex_destroy(&mut1);
    pthread_exit(NULL);
}