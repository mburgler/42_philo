/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:54:29 by mburgler          #+#    #+#             */
/*   Updated: 2023/06/09 13:44:04 by mburgler         ###   ########.fr       */
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

typedef struct s_philo
{
    int no_philo;
    int left_fork;
    int right_fork;
    pthread_t   id;
}       t_philo;

typedef struct s_msc
{
    int nb_philo;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int nb_must_eat;
    t_philo *philo;
}            t_msc;

//philo_main.c
int	*init(int nb_args, char **strs, t_msc *msc);

//ft_error_management.c
void    ft_error(char *str, t_msc *msc);

//philo_utils.c
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_bzero(void *s, size_t n);
int	overflow_handler(const char *str, int i, int sign);
int	ft_atoi(const char *nptr);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
