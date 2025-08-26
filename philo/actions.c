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

void take_forks(t_philo *philo, int right, int left)
{
    t_data *data;
    // int left_fork; 
    // int right_fork;
    
    data = philo->data;
    // left_fork = philo->id - 1; 
    // right_fork = philo->id % data->num_philos;
    if (philo->id - 1 % 2 == 0)
    {
        pthread_mutex_lock(data->forks + left);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(data->forks + right);
        print_action(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(data->forks + right);
        print_action(philo, "has taken a fork");
        pthread_mutex_lock(data->forks + left);
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
    take_forks(philo, right_fork, left_fork);
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = current_timestamp();
    pthread_mutex_unlock(&philo->meal_mutex);
    print_action(philo, "is eating");
    precise_sleep(data->time_to_eat);
    pthread_mutex_unlock(&data->forks[left_fork]);
    pthread_mutex_unlock(&data->forks[right_fork]);
    pthread_mutex_lock(&philo->meal_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);
}

void sleeping(t_philo *philo)
{
    int time;
    long last_meal_copy;

    print_action(philo, "is sleeping");
    if (philo->data->num_philos % 2 == 0)
        precise_sleep(philo->data->time_to_sleep);
    else
    {
        pthread_mutex_lock(&philo->meal_mutex);
        last_meal_copy = philo->last_meal_time;
        pthread_mutex_unlock(&philo->meal_mutex);

        time = max(philo->data->time_to_die - (current_timestamp() - last_meal_copy),
                   philo->data->time_to_sleep);
        precise_sleep(time);
    }
}


void thinking(t_philo *philo)
{
    print_action(philo, "is thinking");   
}
