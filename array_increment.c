/*
	Tarefa:
		Criar duas threads, sem conta com o fluxo main, para incrementar em 1 cada valor
		em um array com 10.000 elementos.
		
	Teste:
		As 10.000 posições são inicializados com 0 e cada posição terá 1 adicinado, logo,
		a soma de todas as posições deve dar 10.000.
		
		Foi criada a função arraySumAllEntries(array, numero de elementos do array) para realizar essa soma.
		Se em algum momento a soma não der 10.000, o algoritmo está incorreto.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int arraySumAllEntries(int *array,int NUMBER_ELEMENTS)
{
	int result = 0;
	for(int i = 0; i < NUMBER_ELEMENTS; i++)
	{
		result += array[i];
	}
	return result;
}

void initArrayWithZeros(int *array,int NUMBER_ELEMENTS)
{
	
	for(int i = 0;i < NUMBER_ELEMENTS ; i++)
	{
		array[i] = 0;
	}
}

void printArrayElements (int *array)
{
	for(int a = 0; a < 100; a++)
	{
		for(int b = 0; b < 100; b++)
		{
			printf("%d",array[b]);
		}
	}
	printf("\n");
}

typedef struct 
{
	int *array,
		 id,
		 limit;
}Entries;

void *increment (void *args)
{
	Entries *entry = (Entries *) args;
	
	if(entry->id)
	{
		for(int i = entry->limit; i <= 10000;i++)
		{
			entry->array[i] += 1;
		}
		free(entry);
		pthread_exit(NULL);	
	}
	
	for(int i = 0; i < entry->limit;i++)
	{
		entry->array[i] += 1;
	}
	
	free(entry);
	pthread_exit(NULL);	
}



int main()
{
	int NUMBER_THREADS = 2;
	int array[10000];
	int NUMBER_OF_ELEMENTS = 10000;
	pthread_t thread_id[NUMBER_THREADS];
	Entries *entry;
	
	initArrayWithZeros(array,NUMBER_OF_ELEMENTS);
	
	for(int i = 0; i < NUMBER_THREADS;i++)
	{
		entry = malloc(sizeof(Entries));	
		if(entry == NULL){ printf("ERRO: malloc\n"); exit(-1);}	
		entry->id = i;	
		entry->array = array;
		entry->limit = 5000;
				
		if(pthread_create(&thread_id[i],NULL,increment,(void*)entry))
		{
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}
	
	for(int thread = 0; thread < NUMBER_THREADS;thread++)
	{
		if(pthread_join(thread_id[thread],NULL))
		{
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		}
	}
	
	//printArrayElements(array);
	int result = arraySumAllEntries(array,10000);
	printf("%d\n",result);
	pthread_exit(NULL);	
}

