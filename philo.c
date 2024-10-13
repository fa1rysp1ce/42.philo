#include "philo.h"

int	meal(t_data *data)
{
	int			i;

	if (pthread_create(&data->obs, NULL, &observer, data))
		return (1);
	if (data->num_philos == 1 && pthread_create(&(data->philos[0].thread),
		 NULL, &routine_one, &(data->philos[0])) != 0)
		return (2);
	else
	{
		while (data->done_flag == 0)
		{
			i = 0;
			while (i < data->num_philos && data->done_flag == 0)
			{
				if (data->philos[i].ifree == 0 && pthread_create(&(
					data->philos[i].thread), NULL, &routine, 
					&(data->philos[i])) != 0)
					return (3);
				usleep(100);
				i++;
			}
		}
	}
	free_pthread(data);
	return (0);
}

static int	die_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->is_eating);
	if (getms() - philo->last_meal == philo->data->die_time)
	{
		pthread_mutex_lock(&philo->data->writing);
		printf("%zu %d died\n", getms(), philo->id + 1);
		pthread_mutex_unlock(&philo->data->writing);
		philo->data->done_flag = 1;
		pthread_mutex_unlock(&philo->is_eating);
		return (1);
	}
	pthread_mutex_unlock(&philo->is_eating);
	return (0);
}

void	*observer(void *arg)
{
	int	i;
	int	finished_count;
	t_data	*data;

	data = arg;
	while (data->done_flag == 0)
	{
		finished_count = 0;
		i = 0;
		while (i < data->num_philos)
		{
			if (die_check(&data->philos[i]) == 1)
				return (data);
			if (data->philos[i].meals_eaten == data->meal_number)
			{
				finished_count++;
				data->philos[i].ifree = 1;
			}
			i++;
		}
		if (finished_count == data->num_philos)
			data->done_flag = 2;
	}
	return (data);
}
