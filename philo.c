/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:20:54 by junruh            #+#    #+#             */
/*   Updated: 2024/10/15 14:51:19 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	meal(t_data *data)
{
	int			i;

	if (pthread_create(&data->obs, NULL, &observer, data))
		return (1);
	if (data->num_philos == 1 && pthread_create(&(data->philos[0].thread),
			NULL, &routine_one, &(data->philos[0])) != 0)
		return (2);
	else if (data->num_philos != 1)
	{
		i = 0;
		while (i < data->num_philos && sim_end(data) == 0)
		{
			if (pthread_create(&(data->philos[i].thread), NULL,
					&routine, &(data->philos[i])) != 0)
				return (3);
			data->philos[i].thread_active = 1;
			usleep(100);
			i++;
		}
	}
	return (0);
}

static int	die_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->is_eating);
	if (getms() - philo->last_meal >= philo->data->die_time)
	{
		pthread_mutex_lock(&philo->data->writing);
		printf("%zu %d died\n", getms(), philo->id + 1);
		pthread_mutex_unlock(&philo->data->writing);
		pthread_mutex_lock(&philo->data->done_m);
		philo->data->done_flag = 1;
		pthread_mutex_unlock(&philo->data->done_m);
		pthread_mutex_unlock(&philo->is_eating);
		return (1);
	}
	pthread_mutex_unlock(&philo->is_eating);
	return (0);
}

void	*observer(void *arg)
{
	int		i;
	int		finished_count;
	t_data	*data;

	data = arg;
	while (sim_end(data) == 0)
	{
		finished_count = 0;
		i = 0;
		while (i < data->num_philos)
		{
			if (die_check(&data->philos[i]) == 1)
				return (data);
			pthread_mutex_lock(&data->done_m);
			if (data->philos[i].meals_eaten == data->meal_number)
				finished_count++;
			pthread_mutex_unlock(&data->done_m);
			i++;
		}
		pthread_mutex_lock(&data->done_m);
		if (finished_count == data->num_philos)
			data->done_flag = 2;
		pthread_mutex_unlock(&data->done_m);
	}
	return (data);
}
