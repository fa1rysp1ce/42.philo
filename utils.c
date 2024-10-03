#include "philo.h"

void	free_pthread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join((data->philos[i].thread), NULL);
		pthread_mutex_destroy(&data->mutex[i]);
		i++;
	}
}

size_t	getms(void)
{
	struct timeval	timev;
	size_t			mstime;

	gettimeofday(&timev, NULL);
	mstime = (timev.tv_sec * 1000) + (timev.tv_usec / 1000); //(timev.tv_sec * 1000) + (timev.tv_usec / 1000);
	return (mstime);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	ispos;

	i = 0;
	res = 0;
	ispos = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
		ispos = 1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	if (ispos == 1)
		res = -res;
	return (res);
}