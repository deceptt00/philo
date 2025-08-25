#include "philo.h"

void print_action(t_philo *philo, const char *action)
{
    long timestamp;
    
    if (!is_running(philo->data))
        return;
    timestamp = current_timestamp() - philo->data->start_time;
    pthread_mutex_lock(&philo->data->print_mutex);
    if (is_running(philo->data))
        printf("%ld %d %s\n", timestamp, philo->id, action);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

void take_forks(t_philo *philo)
{
    t_data *data;
    int left_fork; 
    int right_fork;
    
    data = philo->data;
    left_fork = philo->id - 1; 
    right_fork = philo->id % data->num_philos;
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&data->forks[left_fork]);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[right_fork]);
        print_action(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&data->forks[right_fork]);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[left_fork]);
        print_action(philo, "has taken a fork");
    }
}

void eating(t_philo *philo)
{
    t_data *data;
    int left_fork; 
    int right_fork;
    
    data = philo->data;
    left_fork = philo->id - 1; 
    right_fork = philo->id % data->num_philos;
    take_forks(philo);
    philo->last_meal_time = current_timestamp();
    print_action(philo, "is eating");
    precise_sleep(data->time_to_eat);
    pthread_mutex_unlock(&data->forks[left_fork]);
    pthread_mutex_unlock(&data->forks[right_fork]);
    philo->meals_eaten++;
}

void sleeping(t_philo *philo)
{
    print_action(philo, "is sleeping");
    precise_sleep(philo->data->time_to_sleep);
}

void thinking(t_philo *philo)
{
    print_action(philo, "is thinking");   
}
