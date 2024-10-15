/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:21:42 by junruh            #+#    #+#             */
/*   Updated: 2024/10/14 19:55:18 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_pthread(t_data *data)
{
	int	i;

	i = 0;
	pthread_join(data->obs, NULL);
	while (i < data->num_philos)
	{
		if (data->philos[i].thread_active == 1 || data->num_philos == 1)
			pthread_join((data->philos[i].thread), NULL);
		pthread_mutex_destroy(&data->mutex[i]);
		pthread_mutex_destroy(&data->philos[i].is_eating);
		i++;
	}
	pthread_mutex_destroy(&data->writing);
	pthread_mutex_destroy(&data->done_m);
}

int	sim_end(t_data *data)
{
	int	r;

	r = 0;
	pthread_mutex_lock(&data->done_m);
	if (data->done_flag != 0)
		r = 1;
	pthread_mutex_unlock(&data->done_m);
	return (r);
}

size_t	getms(void)
{
	struct timeval	timev;
	size_t			mstime;

	gettimeofday(&timev, NULL);
	mstime = (timev.tv_sec * 1000) + (timev.tv_usec / 1000);
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
