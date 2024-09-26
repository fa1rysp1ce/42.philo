#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo
{
	pthread_t	thread;
	int			meals_eaten;
	size_t		last_meal;
}	t_philo;

typedef struct s_data
{
	t_philo	*philos;
	size_t	die_time;
	size_t	eat_time;
	size_t	sleep_time;
	size_t	meal_number;
}

#endif