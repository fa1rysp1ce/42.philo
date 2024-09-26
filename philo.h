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
	int			id;
	int			meals_eaten;
	int			is_busy;
	size_t		last_meal;
}	t_philo;

typedef struct s_data
{
	t_philo			philos[200];
	pthread_mutex_t	mutex[200];
	int				num_philos;
	size_t			die_time;
	size_t			eat_time;
	size_t			sleep_time;
	size_t			meal_number;
}	t_data;

//main
int	main(int argc, char **argv);
int	init_all(int argc, char **argv, t_data *data, size_t start_time);
int	check_input(int argc, char **argv);

//utils
size_t	getusecs(void);
int	ft_atoi(const char *str);

#endif