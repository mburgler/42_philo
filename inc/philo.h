/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:54:29 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/04 20:43:51 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

struct	s_msc;

typedef struct s_philo
{
	long long		time_birth;
	long long		time_last_meal;
	int				nb_philo;
	int				left_fork;
	int				*right_fork;
	int				meal_count;
	struct s_msc	*msc;
	pthread_t		thread_id;
}		t_philo;

typedef struct s_mutex
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t	meal_count;
}		t_mutex;

typedef struct s_msc
{
	int		nb_philo;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		nb_must_eat;
	int		mutex_initialised;
	bool	stop_simulation;
	t_philo	**philo;
	t_mutex	*mutex;
}		t_msc;

//philo_main.c
int			init_philo(int i, t_msc *msc);
int			init_mutex(t_msc *msc);
int			init(t_msc *msc);

//parsing.c
int			parsing(int nb_args, char **strs, t_msc *msc);
int			check_max_min_int(char *str, t_msc *msc);
void		parsed_to_variables(int nb_args, t_msc *msc, char **strs);

//simulation_control.c
int			simulation_startup(t_msc *msc, int i);
void		simulation_shutdown(t_msc *msc);
void		*matrix(void *arg);

//simulaton_routine.c
void		philo_eats(t_philo *one_philo, t_msc *msc);
int			philo_sleeps(t_philo *one_philo, t_msc *msc);

//ft_error_management.c
void		ft_err(char *str, t_msc *msc);
void		free_null(void **ptr);
void		free_ma_boi(t_msc *msc);
void		destroy_mutexes(t_msc *msc);

//philo_utils.c
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_bzero(void *s, size_t n);
int			overflow_handler(const char *str, int i, int sign);
int			ft_atoi(const char *nptr);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

//philo_utils2.c
int			ft_strlen(const char *s);
long long	sys_time(void);
int			ft_pthread_join(int j, pthread_t *philo_thread, t_msc *msc);
void		ft_mutex_print(t_msc *msc, t_philo *one_philo, char *message);
void		ft_mutex_print_death(t_msc *msc, t_philo *one_philo);

//ft_itoa.c
int			helper(int n);
void		reverse(char *str);
int			strlen_giver(int n);
char		*ft_itoa(int n);

#endif
