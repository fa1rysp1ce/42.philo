#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_data t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	pthread_mutex_t	is_free;
	size_t			last_meal;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	t_philo	philos[200];
	pthread_mutex_t	mutex[200];
	int				done_flag;
	int				num_philos;
	size_t			die_time;
	size_t			eat_time;
	size_t			sleep_time;
	int				meal_number;
	size_t			start_time;
}	t_data;


//main
int	main(int argc, char **argv);
int	init_all(int argc, char **argv, t_data *data, size_t start_time);
int	check_input(int argc, char **argv);
int	init_philos(t_data *data, t_philo *philos);

//philo
int		meal(t_data *data);
void	*observer(void *arg);
void	*routine(void *arg);
void	free_mutex(t_data *data);

//utils
size_t	getms(void);
int	ft_atoi(const char *str);

#endif