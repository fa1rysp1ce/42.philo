#include "philo.h"

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