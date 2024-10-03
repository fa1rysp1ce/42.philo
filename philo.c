#include "philo.h"

int	meal(t_data *data)
{
	pthread_t	obs;
	int			i;

	pthread_create(&obs, NULL, &observer, data);
	if (data->num_philos == 1)
		pthread_create(&(data->philos[0].thread), NULL, &routine_one, &(data->philos[0]));
	else
	{
		while (data->done_flag == 0)
		{
			i = 0;
			while (i < data->num_philos && data->done_flag == 0)
			{
				if (data->philos[i].meals_eaten != data->meal_number 
					&& data->philos[i].ifree == 0)
					pthread_create(&(data->philos[i].thread), NULL, &routine, &(data->philos[i]));
				usleep(100);
				i++;
			}
		}
	}
	pthread_join(obs, NULL);
	free_pthread(data);
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

void	*routine_one(void *arg)
{
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->data->mutex[philo->fork1]);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	while (philo->data->done_flag == 0)
		usleep(50);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	return (philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	//pthread_mutex_lock(&philo->is_free);
	philo->ifree = 1;
	if (eating(philo))
		return (NULL);
	if (philo->data->done_flag != 0)
	{
	//	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	//	pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
		return (NULL);
	}
	printf("%zu %d is sleeping\n", getms(), philo->id + 1);
	usleep(philo->data->sleep_time * 1000); //hier
	if (philo->data->done_flag != 0)
		return (NULL);
	printf("%zu %d is thinking\n", getms(), philo->id + 1);
	//pthread_mutex_unlock(&philo->is_free);
	philo->ifree = 0;
	return (philo);
}

int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex[philo->fork1]);
	if (philo->data->done_flag != 0)
	{
		pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
		return (1);	
	}
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	pthread_mutex_lock(&philo->data->mutex[philo->fork2]);
	if (philo->data->done_flag != 0)
	{
		pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
		pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
		return (2);
	}
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	philo->last_meal = getms();
	printf("%zu %d is eating\n", philo->last_meal, philo->id + 1);
	usleep((philo->data->eat_time * 1000)); //hier
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
	return (0);
}
