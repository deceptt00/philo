#include "philo.h"

int parse_arguments(int ac, char **av, t_data *data)
{
    if (ac < 5 || ac > 6)
    {
       return (1);
    }
    data->num_philos = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		data->meals_required = ft_atoi(av[5]);
		if (data->meals_required == -1)
			return (write(2, "Error: Invalid argument(s)\n", 27), 1);
	}
	else
		data->meals_required = -1;
	if (data->num_philos <= 0 || data->time_to_die < 0 || \
		data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (write(2, "Error: Invalid argument(s)\n", 27), 1);
    data->running = 1;
    data->start_time = current_timestamp();
    
    return (0);
}
