/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:54:29 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/14 19:50:20 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <string.h>
# include <pthread.h>
#include <stdbool.h>

typedef struct s_philo
{
    int nb_philo;
    int left_fork;
    int *right_fork;
	int	meal_count;
	long	last_meal;
    bool    dead;
    pthread_t   thread_id;
}       t_philo;

typedef struct s_mutex
{
	pthread_mutex_t *forks;
	pthread_mutex_t print;
	pthread_mutex_t death;
	pthread_mutex_t meal_count;
}       t_mutex;

typedef struct s_msc
{
    int nb_philo;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int nb_must_eat;
	int	mutex_initialised;
    t_philo **philo;
	t_mutex *mutex;
}            t_msc;

//philo_main.c
int	parsing(int nb_args, char **strs, t_msc *msc);
int    init_philo(int i, t_msc *msc);
int	init(t_msc *msc);

//ft_error_management.c
void    ft_error(char *str, t_msc *msc);
void	free_and_nullify(void **ptr);
void	free_ma_boi(t_msc *msc);
void	destroy_mutexes(t_msc *msc);

//philo_utils.c
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_bzero(void *s, size_t n);
int	overflow_handler(const char *str, int i, int sign);
int	ft_atoi(const char *nptr);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

//philo_utils2.c
int	ft_strlen(const char *s);

#endif
