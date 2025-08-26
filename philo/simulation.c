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
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = data->start_time;
	pthread_mutex_unlock(&philo->meal_mutex);
    if (philo->id % 2 == 0)
        usleep(500);
    if (philo->id == data->num_philos && data->num_philos % 2 == 1)
    {
        usleep(700);	
    }
	while (is_running(data))
	{
		eating(philo);
		if (!is_running(data))
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

int check_philosopher_death(t_data *data)
{
    int i = 0;
    long last_meal, current_time;
    
    while (i < data->num_philos)
    {
        pthread_mutex_lock(&data->philos[i].meal_mutex);
        last_meal = data->philos[i].last_meal_time;        
        current_time = current_timestamp();
        if (current_time - last_meal > data->time_to_die)
        {
            print_action(&data->philos[i], "died");
            stop_simulation(data);
        	pthread_mutex_unlock(&data->philos[i].meal_mutex);
            return (1);
        }
        pthread_mutex_unlock(&data->philos[i].meal_mutex);
        i++;
    }
    return (0);
}

int check_meals_satisfaction(t_data *data)
{
    int i;
    int satisfied_count;

    if (data->meals_required == -1)
        return (0);
    satisfied_count = 0;
    i = 0;
    while (i < data->num_philos)
    {
        pthread_mutex_lock(&data->philos[i].meal_mutex);
        if (data->philos[i].meals_eaten >= data->meals_required)
            satisfied_count++;
        pthread_mutex_unlock(&data->philos[i].meal_mutex);
        i++;
    }
    if (satisfied_count == data->num_philos)
    {
        stop_simulation(data);
        return (1);
    }
    return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (is_running(data))
	{
		if (check_philosopher_death(data))
			break;
		if (check_meals_satisfaction(data))
			break;
		usleep(1000);
	}
	return (NULL);
}

int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_create(&data->philos[i].thread, NULL,
			philosopher_routine, &data->philos[i]);
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
	    pthread_mutex_init(&data->philos[i].meal_mutex, NULL);
		i++;
	}
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor_thread;
	pthread_mutex_init(&data->monitor_mutex, NULL);
	initialize_philosophers(data);
    data->running = 1;
	create_philosopher_threads(data);
	pthread_create(&monitor_thread, NULL, monitor_routine, data);
	join_philosopher_threads(data);
	pthread_join(monitor_thread, NULL);
	return (0);
}