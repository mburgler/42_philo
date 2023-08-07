/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 17:55:38 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/07 18:14:57 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	one_philo(t_msc *msc)
{
	pthread_t	thr[1];	
	int			i;

	i = 0;
	msc->philo[i]->time_birth = sys_time();
	pthread_mutex_lock(&msc->mutex->death);
	msc->philo[i]->time_last_meal = msc->philo[i]->time_birth;
	pthread_mutex_unlock(&msc->mutex->death);
	if (pthread_create(&thr[i], NULL, one_philo_routine, msc) != 0)
	{
		if (ft_pthread_join(i, thr, msc) == -1)
			return (ft_err("pthread_create and pthread_join", msc), -1);
		else
			return (ft_err("pthread_create", msc), -1);
	}
	pthread_mutex_lock(&msc->mutex->death);
	while(msc->stop_simulation == false)
	{
		pthread_mutex_unlock(&msc->mutex->death);
		usleep(5000);
		pthread_mutex_lock(&msc->mutex->death);
	}
	pthread_mutex_unlock(&msc->mutex->death);
	if (ft_pthread_join(i, thr, msc) == -1)
		return (ft_err("pthread_join", msc), -1);
	return (0);
}

void	*one_philo_routine(void *arg)
{
	t_msc	*msc;

	if (!arg)
		return (NULL);
	msc = (t_msc *)arg;
	pthread_mutex_lock(&msc->mutex->forks[0]);
	ft_mutex_print(msc, msc->philo[0], "has taken his left fork");
	usleep(msc->time_to_die * 1000);
	pthread_mutex_unlock(&msc->mutex->forks[0]);
	ft_mutex_print_death(msc, msc->philo[0]);
	msc->stop_simulation = true;
	return (NULL);
}