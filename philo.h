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
	pthread_mutex_t	is_eating;
	int				ifree;
	size_t			last_meal;
	t_data			*data;
	int				fork1;
	int				fork2;
}	t_philo;

typedef struct s_data
{
	t_philo			philos[200];
	pthread_mutex_t	mutex[200];
	pthread_mutex_t	writing;
	pthread_t		obs;
	int				done_flag;
	int				num_philos;
	int				meal_number;
	size_t			die_time;
	size_t			eat_time;
	size_t			sleep_time;
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

//routine
void	*routine(void *arg);
int		wait_turn(t_philo *philo);
void	eat(t_philo *philo);
void	*routine_one(void *arg);

//utils
void	free_pthread(t_data *data);
size_t	getms(void);
int		ft_atoi(const char *str);

#endif