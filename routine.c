/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junruh <junruh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:21:31 by junruh            #+#    #+#             */
/*   Updated: 2024/10/15 14:53:25 by junruh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine_one(void *arg)
{
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->data->mutex[philo->fork1]);
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	while (sim_end(philo->data) == 0)
		usleep(50);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	return (philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (sim_end(philo->data) == 0)
	{
		if (wait_turn(philo))
			return (NULL);
		if (sim_end(philo->data) != 0)
			return (NULL);
		pthread_mutex_lock(&philo->data->writing);
		printf("%zu %d is sleeping\n", getms(), philo->id + 1);
		pthread_mutex_unlock(&philo->data->writing);
		usleep(philo->data->sleep_time * 1000);
		if (sim_end(philo->data) != 0)
			return (NULL);
		pthread_mutex_lock(&philo->data->writing);
		printf("%zu %d is thinking\n", getms(), philo->id + 1);
		pthread_mutex_unlock(&philo->data->writing);
	}
	return (philo);
}

int	wait_turn(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex[philo->fork1]);
	if (sim_end(philo->data) != 0)
	{
		pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
		return (1);
	}
	pthread_mutex_lock(&philo->data->writing);
	printf("%zu %d has taken a fork\n", getms(), philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	pthread_mutex_lock(&philo->data->mutex[philo->fork2]);
	if (sim_end(philo->data) != 0)
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
	if (sim_end(philo->data) == 0)
		printf("%zu %d is eating\n", philo->last_meal, philo->id + 1);
	pthread_mutex_unlock(&philo->data->writing);
	pthread_mutex_unlock(&philo->is_eating);
	usleep((philo->data->eat_time * 1000));
	pthread_mutex_lock(&philo->data->done_m);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->done_m);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork1]);
	pthread_mutex_unlock(&philo->data->mutex[philo->fork2]);
}
