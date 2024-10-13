#include "philo.h"

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
	philo->ifree = 1;
	if (wait_turn(philo))
		return (NULL);
	if (philo->data->done_flag != 0)
		return (NULL);
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d is sleeping\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	usleep(philo->data->sleep_time * 1000);
	if (philo->data->done_flag != 0)
		return (NULL);
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d is thinking\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	philo->ifree = 0;
	return (philo);
}

int	wait_turn(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex[philo->fork1]);
	if (philo->data->done_flag != 0)
	{
		pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
		return (1);	
	}
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	pthread_mutex_lock(&philo->data->mutex[philo->fork2]);
	if (philo->data->done_flag != 0)
	{
		pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
		pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
		return (2);
	}
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	eat(philo);
	return (0);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->is_eating);
	philo->last_meal = getms();
	pthread_mutex_lock(&philo->data->writing);
	if (philo->data->done_flag == 0)
		printf("%zu %d is eating\n", philo->last_meal, philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	pthread_mutex_unlock(&philo->is_eating);
	usleep((philo->data->eat_time * 1000));
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
}
