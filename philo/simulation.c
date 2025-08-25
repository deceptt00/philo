#include "philo.h"

int is_running(t_data *data)
{
    int status;
    
    pthread_mutex_lock(&data->monitor_mutex);
    status = data->running;
    pthread_mutex_unlock(&data->monitor_mutex);
    
    return (status);
}

void stop_simulation(t_data *data)
{
    pthread_mutex_lock(&data->monitor_mutex);
    data->running = 0;
    pthread_mutex_unlock(&data->monitor_mutex);
}
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	philo->last_meal_time = data->start_time;
    if (philo->id % 2 == 0)
        precise_sleep(data->time_to_eat / 8);
    if (philo->id == data->num_philos && data->num_philos % 2 == 1)
    {
        precise_sleep(data->time_to_eat / 10);
    }
	while (is_running(data))
	{
		eating(philo);
		if (data->meals_required != -1
			&& philo->meals_eaten >= data->meals_required)
			break ;
		if (!is_running(data))
			break ;
		sleeping(philo);
		if (!is_running(data))
			break ;
		thinking(philo);
	}
    return (NULL);
}

int	check_philosopher_death(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (current_timestamp() - data->philos[i].last_meal_time
			> data->time_to_die)
		{
			print_action(&data->philos[i], "died");
			stop_simulation(data);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_meals_satisfaction(t_data *data, int *consecutive_checks)
{
	int	i;
	int	satisfied_count;

	if (data->meals_required == -1)
		return (0);
	satisfied_count = 0;
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten >= data->meals_required)
			satisfied_count++;
		i++;
	}
	if (satisfied_count >= data->num_philos)
	{
		(*consecutive_checks)++;
		if (*consecutive_checks >= 3)
		{
			stop_simulation(data);
			return (1);
		}
	}
	else
		*consecutive_checks = 0;
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		consecutive_satisfied_checks;

	data = (t_data *)arg;
	consecutive_satisfied_checks = 0;
	while (is_running(data))
	{
		if (check_philosopher_death(data))
			return (NULL);
		if (check_meals_satisfaction(data, &consecutive_satisfied_checks))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}

int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d\n", i + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void	join_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	initialize_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].data = data;
		data->philos[i].last_meal_time = data->start_time;
		i++;
	}
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor_thread;

	if (pthread_mutex_init(&data->monitor_mutex, NULL) != 0)
		return (1);
	initialize_philosophers(data);
	if (create_philosopher_threads(data) != 0)
		return (1);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	join_philosopher_threads(data);
	pthread_join(monitor_thread, NULL);
	return (0);
}