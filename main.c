#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc > 6 || argc < 5)
	{
		printf("Error\nMust have 4 or 5 args\n");
		return (1);
	}
	data = init_all(argc, argv);
}

t_data	*init_all(int argc, char **argv)
{
	
}