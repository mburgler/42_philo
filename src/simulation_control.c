/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_control.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 14:40:48 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/04 17:42:56 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	simulation_startup(t_msc *msc, int i)
{
	pthread_t	*threads;

	threads = ft_calloc(sizeof(pthread_t), msc->nb_philo);
	if (!threads)
		return (ft_err("malloc failed", msc), -1);
	while (++i < msc->nb_philo)
	{
		msc->philo[i]->time_birth = sys_time();
		pthread_mutex_lock(&msc->mutex->death);
		msc->philo[i]->time_last_meal = msc->philo[i]->time_birth;
		pthread_mutex_unlock(&msc->mutex->death);
		if (pthread_create(&threads[i], NULL, matrix, msc->philo[i]) != 0)
		{
			if (ft_pthread_join(i, threads, msc) == -1)
				return (free_null((void **)&threads),
					ft_err("pthread_create and pthread_join failed", msc), -1);
			else
				return (free_null((void **)&threads),
					ft_err("pthread_create failed", msc), -1);
		}
	}
	simulation_shutdown(msc);
	if (ft_pthread_join(i, threads, msc) == -1)
		return (ft_err("pthread_join failed", msc), -1);
	return (free_null((void **)&threads), 0);
}

//SHORTENED VERSION:
// void	simulation_shutdown(t_msc *msc)
// {
// 	int		i;
// 	bool	all_ate;

// 	pthread_mutex_lock(&msc->mutex->death);
// 	while (msc->stop_simulation == false)
// 	{
// 		all_ate = true;
// 		i = -1;
// 		while (++i < msc->nb_philo && msc->stop_simulation == false)
// 		{
// 			if (sys_time() - msc->philo[i]->time_last_meal > msc->time_to_die)
// 			{
// 				msc->stop_simulation = true;
// 				return (pthread_mutex_unlock(&msc->mutex->death), 
// 					ft_mutex_print_death(msc, msc->philo[i]));
// 			}
// 			pthread_mutex_lock(&msc->mutex->meal_count);
// 			if (msc->philo[i]->meal_count < msc->nb_must_eat
// 				|| msc->nb_must_eat == -1)
// 				all_ate = false;
// 			pthread_mutex_unlock(&msc->mutex->meal_count);
// 		}
// 		if (all_ate == true)
// 		{
// 			msc->stop_simulation = true;
// 			return ;
// 		}
// 		pthread_mutex_unlock(&msc->mutex->death);
// 		usleep(1000);
// 		pthread_mutex_lock(&msc->mutex->death);
// 	}
// 	pthread_mutex_unlock(&msc->mutex->death);
// }

void	simulation_shutdown(t_msc *msc)
{
	int	i;
	int	all_ate;

	pthread_mutex_lock(&msc->mutex->death);
	while (msc->stop_simulation == false)
	{
		all_ate = 1;
		i = -1;
		pthread_mutex_unlock(&msc->mutex->death);
		pthread_mutex_lock(&msc->mutex->death);
		while (++i < msc->nb_philo && msc->stop_simulation == false)
		{
			all_ate = inner_loop(msc, i, all_ate);
			if (all_ate == -1)
				return ;
		}
		pthread_mutex_unlock(&msc->mutex->death);
		if (all_ate == 1)
			return (stop_simulation_true(msc));
		usleep(1000);
		pthread_mutex_lock(&msc->mutex->death);
	}
	pthread_mutex_unlock(&msc->mutex->death);
}

void	stop_simulation_true(t_msc *msc)
{
	pthread_mutex_lock(&msc->mutex->death);
	msc->stop_simulation = true;
	pthread_mutex_unlock(&msc->mutex->death);
}

int	inner_loop(t_msc *msc, int i, int all_ate)
{
	if (sys_time() - msc->philo[i]->time_last_meal > msc->time_to_die)
	{
		msc->stop_simulation = true;
		pthread_mutex_unlock(&msc->mutex->death);
		ft_mutex_print_death(msc, msc->philo[i]);
		return (-1);
	}
	else
		pthread_mutex_unlock(&msc->mutex->death);
	pthread_mutex_lock(&msc->mutex->meal_count);
	if (msc->philo[i]->meal_count < msc->nb_must_eat
		|| msc->nb_must_eat == -1)
		all_ate = 0;
	pthread_mutex_unlock(&msc->mutex->meal_count);
	pthread_mutex_lock(&msc->mutex->death);
	return (all_ate);
}
