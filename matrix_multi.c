/*
	Tarefa: 
	
	Criar um algoritmo que multiplique matrizes, ambas as matrizes são inicializadas dentro da main com valores random.
	O numero de threads e dimensão das matrizes (que são quadradas) são dados como entrada do script.
	
	Taks:
	
	To create an algorithm that multiply two matrices, both are initiallize in main with random numbers.
	The number of threads and dimension of the matrices(both square) are given as entries of the script.
	
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

float *matrix_A,*matrix_B,*matrix_result;
int matrix_dimension,number_of_threads;
	
void initSquareMatrixWithRandomNumbers(float *A,int matrix_dimension)
{
	
	for(int i = 0; i < matrix_dimension; i++)
	{
		for(int j = 0; j < matrix_dimension;j++)
		{
			A[i * matrix_dimension + j]= rand() % 5;
		}
	}	
}

void  allocateMemorySquareMatrix(float **A,int matrix_dimension)
{	
	*A =   malloc(sizeof(float) * matrix_dimension * matrix_dimension);		
}

void showMatrix(float *A,int dimension)
{
	puts("");
	for(int i = 0; i < dimension; i++)
	{
		for(int j = 0; j < dimension;j++)
		{
			printf("%.2f ",A[i * dimension +j]);
		}
		puts("");
	}
	puts("");
}

// for sequential application
void matrixMultiplication(float *A, float *B,float *result,int matrix_dimension)
{		
	for(int j = 0; j <matrix_dimension;j++)
	{
		for(int i = 0; i < matrix_dimension;i++)
		{
			  int temp = 0;
			  for(int aux = 0; aux < matrix_dimension; aux++)
			  {
			  	temp += A[i * matrix_dimension + aux] * B[aux * matrix_dimension + j] ;
			  }
			  result[i * matrix_dimension + j] = temp;
		}		
	}
}

typedef struct 
{
	int id;
}Entry;

void * multiplication(void *args)
{
	Entry *thread_id = (Entry *) args;
	int temp;
	for(int j = 0; j <matrix_dimension;j++)
	{
		for(int i = thread_id->id ; i < matrix_dimension;i+= number_of_threads)
		{
			  temp = 0;
			  for(int aux = 0; aux < matrix_dimension; aux++)
			  {
			  	temp += matrix_A[i * matrix_dimension + aux] * matrix_B[aux * matrix_dimension + j] ;
			  }
			  matrix_result[i * matrix_dimension + j] = temp;
		}		
	}
	pthread_exit(NULL);
}
	




int main(int argc, char* argv[])
{	
	pthread_t *system_id;
	Entry *thread_id;
	srand(time(NULL)); // reset rand(),every execution generate  new random numbers
	
	if(argc < 3)
	{
		printf(" Enter: %s <matrix's dimension>  <Threads>\n",argv[0]); return 666;
	}
	
	matrix_dimension    = atoi(argv[1]);
	number_of_threads   = atoi(argv[2]);
	
	//allocation of memory
	allocateMemorySquareMatrix(&matrix_A,matrix_dimension);
	if(matrix_A == NULL){printf("ERROR: couldnt allocate space on malloc\n"); return 666;}
	
	allocateMemorySquareMatrix(&matrix_B,matrix_dimension);
	if(matrix_B == NULL){printf("ERROR: couldnt allocate space on malloc\n"); return 666;}
	
	allocateMemorySquareMatrix(&matrix_result,matrix_dimension);
	if(matrix_result == NULL){printf("ERROR: couldnt allocate space on malloc\n"); return 666;}
	
	system_id = malloc(sizeof(pthread_t) * number_of_threads);
	if(system_id == NULL){printf("ERROR: couldnt allocate space on malloc\n"); return 666;}
	
	thread_id = malloc(sizeof(Entry) * number_of_threads);
	if(thread_id == NULL){printf("ERROR: couldnt allocate space on malloc\n"); return 666;}
	
	//inicialization of the matrix with random numbers
	initSquareMatrixWithRandomNumbers(matrix_A,matrix_dimension);
	initSquareMatrixWithRandomNumbers(matrix_B,matrix_dimension);
	
	//create threads and pass as argument for which one of them their id
	for(int i = 0; i < number_of_threads;i++)
	{
		(thread_id + i)->id = i;
		if(pthread_create(system_id + i,NULL, multiplication, (void *) (thread_id + i)))
		{
			puts("ERRO--pthread_create");return 666;
		}
	}
	// Wait all threads complete the task then allow the main to end
	for(int i = 0; i < number_of_threads; i++)
	{
		if(pthread_join(system_id[i],NULL)){printf("--ERRO: pthread_join() \n"); exit(666);}
	}	
	
	// print  A*B = RESULT
	
	showMatrix(matrix_A,matrix_dimension);
	showMatrix(matrix_B,matrix_dimension);
	showMatrix(matrix_result,matrix_dimension);
	
	//free all variables allocated dynamically
	free(matrix_A);
	free(matrix_B);
	free(matrix_result);
	free(system_id);
	free(thread_id);	
	return 0;
}
