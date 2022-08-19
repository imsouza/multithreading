#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;

/**
 * Realiza o somatório de 1 até um dado valor passado por parâmetro.
 * A função runner será utilizada como uma passagem de alta ordem para a thread,
 * ou seja, esta rotina que será chamada durante as threads de execução para
 * realização do somatório de valores.
 */
void *runner (void *param) {
	int i, upper = atoi(param);

	// A cada iteração o valor da variável global 'sum' é incrementada em i
	sum = 0;
	for (i = 1; i <= upper; i++)
		sum += i;

	/**
	 * Descrição:
	 * A função pthread_exit() permite ao programador especificar o termino de 
	 * uma thread e caso a thread se finalize normalmente
	 */
	pthread_exit(0);
}



int main (int argc, char *argv []) {
	pthread_t tid;
	pthread_attr_t attr;

	if (argc != 2)
	{
		fprintf (stderr, "usage: ./main <integer value>\n");
		return -1;
	}

	if (atoi (argv [1]) < 0)
	{
		fprintf (stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}


	/**
	 * Descrição:
	 * A função pthread_attr_init inicializa um atributo de objeto da thread
	 *
	 * Argumentos:
	 * &attr: Argumento único que definirá os atributos default da thread. Este
	 * argumento define o conjunto de valores para serem usados durante a criação 
	 * da thread
	 */
	pthread_attr_init (&attr);

	/**
	 * Descrição:
	 * A função pthread_create permite a passagem de um argumentos paraa thread, 
	 * criando, assim, uma nova thread
	 *
	 * Argumentos:
	 * &tid: Referência única para uma nova thread
	 * &attr: Referência para especificar atributos de uma nova thread
	 * runner: Função de alta ordem que a thread irá executar
	 * argv[1]: Argumento que obterá o valor inteiro passado pelo terminal
	 */
	pthread_create (&tid, &attr, runner, argv[1]);


	/**
	 * Descrição:
	 * O join permite realizar a sincronização entre threads. A função pthread_join
	 * faz com que a thread que o chamou esperar até que a thread especificada encerre
	 *
	 * Argumentos:
	 * tid: Argumento referente ao id da thread
	 * NULL: Ponteiro NULL indicando o status
	 */
	pthread_join (tid, NULL);

	printf ("sum = %d\n", sum);
	return 0;
}