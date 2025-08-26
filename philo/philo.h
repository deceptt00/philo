#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
    int             id;
    int             meals_eaten;
    long            last_meal_time;
    pthread_t       thread;
    struct s_data   *data;
    pthread_mutex_t meal_mutex;
}               t_philo;

typedef struct s_data
{
    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             meals_required;
    long            start_time;
    int             running;
    
    t_philo         *philos;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t monitor_mutex;
}               t_data;


// utils.c
long    current_timestamp(void);
void    precise_sleep(long duration);
int     ft_atoi(const char *str);
int     max(int a, int b);


// parsing.c
int     parse_arguments(int ac, char **av, t_data *data);

// actions.c
void    print_action(t_philo *philo, const char *action);
void    take_forks(t_philo *philo, int right, int left);
void    eating(t_philo *philo);
void    sleeping(t_philo *philo);
void    thinking(t_philo *philo);

// simulation.c
int     is_running(t_data *data);
void    stop_simulation(t_data *data);
void    *philosopher_routine(void *arg);
void    *monitor_routine(void *arg);
int     start_simulation(t_data *data);

// main.c
int     init_data(t_data *data);
void    cleanup_data(t_data *data);

#endif