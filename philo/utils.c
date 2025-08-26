#include "philo.h"


int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

long current_timestamp(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void precise_sleep(long time)
{
	long	start;
	long	now;
	long	diff;

	start = current_timestamp();
	now = start;
	diff = 0;
	while (diff < time)
	{
		usleep(100);
		now = current_timestamp();
		diff = now - start;
	}
}

int ft_atoi(const char *str)
{
	int i;
	int sign;
	long result;

	i = 0;
	sign = 1;
	result = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result * sign > 2147483647)
			return (-1);
		if (result * sign < -2147483648)
			return (0);
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return ((int)(result * sign));
}