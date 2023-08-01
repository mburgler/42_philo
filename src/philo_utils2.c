/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:26:00 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/01 17:08:12 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_strlen(const char *s)
{
	unsigned long int	index;

	index = 0;
	while (s[index] != '\0')
	{
		index++;
	}
	return (index);
}

long long	sys_time(void)
{
	struct timeval	time;
	long long		time_ms;

	gettimeofday(&time, NULL);
	time_ms = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (time_ms);
}
//long long to avoid the Year 2038 Problem

int	ft_pthread_join(int threads_created, pthread_t *philo_thread, t_msc *msc)
{
	int	i;

	i = 0;
	while (i < threads_created)
	{
		if (pthread_join(philo_thread[i], NULL) != 0)
			return (-1);
		i++;
	}
	(void)msc;
	return (0);
}

void	ft_mutex_print(t_msc *msc, t_philo *one_philo, char *message)
{
	long long	time_now;

	time_now = sys_time();
	pthread_mutex_lock(&msc->mutex->death);
	if (msc->stop_simulation == true)
	{
		pthread_mutex_unlock(&msc->mutex->death);
		return ;
	}
	pthread_mutex_unlock(&msc->mutex->death);
	pthread_mutex_lock(&msc->mutex->print);
	printf("%lld %d %s\n", (time_now - one_philo->time_birth),
		one_philo->nb_philo, message);
	pthread_mutex_unlock(&msc->mutex->print);
}

void	ft_mutex_print_death(t_msc *msc, t_philo *one_philo)
{
	long long	time_now;

	time_now = sys_time();
	pthread_mutex_lock(&msc->mutex->print);
	printf("%lld %d died\n", (time_now - one_philo->time_birth),
		one_philo->nb_philo);
	pthread_mutex_unlock(&msc->mutex->print);
	if (one_philo->msc->nb_philo == 1)
		pthread_mutex_unlock(&msc->mutex->forks[one_philo->left_fork]);
}
