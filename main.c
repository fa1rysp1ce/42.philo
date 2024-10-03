#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	size_t	start_time;

	start_time = getms();
	if (argc > 6 || argc < 5)
	{
		printf("Error\nMust have 4 or 5 args\n");
		return (1);
	}
	if (init_all(argc, argv, &data, start_time) != 0)
		return (1);
	meal(&data);
	return (0);
}

int	init_all(int argc, char **argv, t_data *data, size_t start_time)
{
	if (check_input(argc, argv) == 1)
	{
		printf("Error\nWrong input\n");
		return (1);
	}
	data->num_philos = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	printf("%zu\n", data->eat_time);
	data->sleep_time = ft_atoi(argv[4]);
	data->start_time = start_time;
	data->done_flag = 0;
	if (argc == 6)
		data->meal_number = ft_atoi(argv[5]);
	else
		data->meal_number = -1;
	printf("%zu\n", start_time);
	if (init_philos(data, data->philos) != 0)
		return (2);
	return (0);
}

int	check_input(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != 0)
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	if (ft_atoi(argv[1]) > 200)
		return (1);
	return (0);
}

int	init_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = getms();
		philos[i].data = data;
		philos[i].ifree = 0;
		if (pthread_mutex_init(&data->mutex[i], NULL) != 0
			|| pthread_mutex_init(&philos[i].is_free, NULL) != 0)
		{
			while (i > 0)
			{
				pthread_mutex_destroy(&data->mutex[i]);
				i--;
			}
			printf("Error\nMutex init failed\n");
			return (1);
		}
		/*if (i % 2 == 0)
		{
			philos[i].fork1 = (i + 1) % data->num_philos;
			philos[i].fork2 = i;
		}
		else
		{*/
			philos[i].fork1 = i;
			philos[i].fork2 = (i + 1) % data->num_philos;
		//}
		i++;
	}
	return (0);
}
