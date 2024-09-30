#include "philo.h"

int	meal(t_data *data)
{
	pthread_t	obs;
	int			i;

	pthread_create(&obs, NULL, &observer, data);
	printf("1\n");
	while (data->done_flag == 0)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (data->done_flag != 0)
				break;
			pthread_create(&(data->philos[i].thread), NULL, &routine, &(data->philos[i]));
			i++;
		}
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join((data->philos[i].thread), NULL);
		i++;
	}
	pthread_join(obs, NULL);
	free_mutex(data);
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
			if (getms() - data->philos[i].last_meal == data->die_time)
			{
				printf("%zu %d died\n", getms(), data->philos[i].id + 1);
				data->done_flag = 1;
				return (data);
			}
			if (data->philos[i].meals_eaten == data->meal_number)
				finished_count++;
			i++;
		}
		if (finished_count == data->num_philos)
			data->done_flag = 2;
	}
	return (data);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->is_free);
	pthread_mutex_lock(&philo->data->mutex[philo->id]);
	if (philo->data->done_flag != 0)
		return (philo);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	if (philo->id == 0)
		pthread_mutex_lock(&philo->data->mutex[philo->data->num_philos - 1]);
	else
		pthread_mutex_lock(&philo->data->mutex[philo->id - 1]);
	if (philo->data->done_flag != 0)
		return (philo);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	philo->last_meal = getms();
	if (philo->data->done_flag != 0)
		return (philo);
	printf("%zu %d is eating\n", philo->last_meal, philo->id + 1);
	usleep(philo->data->eat_time);
	//unlock mutexes, maybe init forks to philos
	philo->meals_eaten++;
	if (philo->data->done_flag != 0)
		return (philo);
	printf("%zu %d is sleeping\n", philo->last_meal, philo->id + 1);
	usleep(philo->data->sleep_time);
	if (philo->data->done_flag != 0)
		return (philo);
	printf("%zu %d is thinking\n", philo->last_meal, philo->id + 1);
	pthread_mutex_unlock(&philo->is_free);
	return (philo);
}

void	free_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->mutex[i]);
		i++;
	}
}