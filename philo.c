#include "philo.h"

int	meal(t_data *data)
{
	pthread_t	obs;
	int			i;

	pthread_create(&obs, NULL, &observer, NULL);
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (data->done_flag != 0)
			//lolololol
			pthread_create(&data->philos[i]->thread, NULL, &routine, data->philos[i]);
		}
	}
}

void	observer(t_data *data)
{
	int	i;
	int	finished_count;

	while (data->done_flag == 0)
	{
		finished_count = 0;
		while (i < data->num_philos)
		{
			if (getms() - data->philos[i]->last_meal == data->die_time)
			{
				printf("%zu %d died\n", getms(), philo->id + 1);
				data->done_flag = 1;
				return ;
			}
			if (data->philos[i]->meals_eaten == data->meal_number)
				finished_count++;
			i++;
		}
		if (finished_count == data->num_philos)
			data->done_flag = 2;
	}
}

void	routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->is_free);
	pthread_mutex_lock(&philo->data->mutex[philo->id]);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	if (philo->id == 0)
		pthread_mutex_lock(&philo->data->mutex[philo->data->num_philos - 1]);
	else
		pthread_mutex_lock(&philo->data->mutex[philo->id - 1]);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	philo->last_meal = getms();
	printf("%zu %d is eating\n", philo->last_meal, philo->id + 1);
	usleep(philo->data->eat_time);
	philo->meals_eaten++;
	printf("%zu %d is sleeping\n", philo->last_meal, philo->id + 1);
	usleep(philo->data->sleep_time);
	printf("%zu %d is thinking\n", philo->last_meal, philo->id + 1);
	pthread_mutex_unlock(&philo->is_free);
}
