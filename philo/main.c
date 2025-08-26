#include "philo.h"

int init_data(t_data *data)
{
    int i;
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philos)
        return (1);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    if (!data->forks)
    {
        free(data->philos);
        return (1);
    }
    pthread_mutex_init(&data->print_mutex, NULL);
    i = 0;
    while (i < data->num_philos)
    {
        data->philos[i].id = i + 1;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = 0;
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    return (0);
}

void cleanup_data(t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->num_philos)
    {
        pthread_mutex_destroy(&data->forks[i]);
        pthread_mutex_destroy(&data->philos[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->monitor_mutex);
    free(data->forks);
    free(data->philos);
}

int main(int ac, char **av)
{
    t_data data;
    
    if (parse_arguments(ac, av, &data) != 0)
        return (1);
    
    if (data.num_philos == 1)
    {
        printf("0 1 has taken a fork\n");
        precise_sleep(data.time_to_die);
        printf("%d 1 died\n", data.time_to_die);
        return (0);
    }
        if (init_data(&data) != 0)
        return (1);
    if (start_simulation(&data) != 0)
    {
        cleanup_data(&data);
        return (1);
    }
    cleanup_data(&data);
    
    return (0);
}
