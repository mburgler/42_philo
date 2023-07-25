/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:22:35 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/25 22:12:23 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	simulation_startup(t_msc* msc)
{
	pthread_t *philo_thread; //pthread_t	philo_thread[4242];
	int	i;

	i = -1;
	philo_thread = ft_calloc(sizeof(pthread_t) * (msc->nb_philo + 1), 1);
	while(++i <= msc->nb_philo)
	{
		pthread_mutex_lock(&msc->mutex->death);
		msc->philo[i]->time_birth = sys_time();
		msc->philo[i]->time_last_meal = msc->philo[i]->time_birth;
		pthread_mutex_unlock(&msc->mutex->death);
		printf("DEBUG NO_PHILO#, no %d\n", msc->philo[i]->nb_philo);
		if (pthread_create(&philo_thread[i], NULL, \
			simulation_running, msc->philo[i]) != 0)
		{
			if(ft_pthread_join(i, philo_thread, msc) == -1)
				return (ft_error("pthread_create and pthread_join failed", msc), -1);
			else
				return (ft_error("pthread_create failed", msc), -1);
		}
	}
	simulation_shutdown(msc);
	printf("*** BEFORE THREAD_JOIN\n");
	if(ft_pthread_join(i, philo_thread, msc) == -1) //< or <= in pthread join -> solved it is <
	 	return (ft_error("pthread_join failed", msc), -1);
	printf("*** AFTER THREAD_JOIN\n");
	free_and_nullify((void **)&philo_thread);
	return (0);
}

void	simulation_shutdown(t_msc *msc)
{
	int i;
	bool all_ate;

	while(msc->stop_simulation == false)
	{
		all_ate = true;
		i = 0;
		while(i++ <= msc->nb_philo) // < or <= since 0-indexed
		{
			if(msc->philo[i]->dead == true)
				msc->stop_simulation = true;
			if(msc->philo[i]->meal_count <= msc->nb_must_eat)
				all_ate = false;
		}
		if(all_ate == true)
			msc->stop_simulation = true;
	}
	return ;
}

//check if philo died thinking is fast enough

void	*simulation_running(void *arg)
{
	t_philo *one_philo;

	if (!arg)
		return (NULL);
	one_philo = (t_philo *)arg;
	while(one_philo->msc->stop_simulation == false)
	{
		philo_eats(one_philo, one_philo->msc);
		philo_sleeps(one_philo, one_philo->msc);
	}
	//printf("DEBUG ROUTINE########################\n");
	//printf("DEBUG ROUTINE#, thread_philo no %d\n", one_philo->nb_philo);
	//pthread_mutex_lock(&one_philo->msc->mutex->print);
	printf("DEBUG ROUTINE#, thread_philo no %d\n", one_philo->nb_philo);
	//pthread_mutex_unlock(&one_philo->msc->mutex->print);
	return(0);
}

void	philo_eats(t_philo *one_philo, t_msc *msc)
{
	
}

void	philo_sleeps(t_philo *one_philo, t_msc *msc)
{
	long long time_now;

	if(msc->stop_simulation == true)
		return ;
	time_now = sys_time();
	if(time_now - one_philo->time_last_meal > msc->time_to_die)
	{
		pthread_mutex_lock(&msc->mutex->death);
		one_philo->dead = true;
		pthread_mutex_unlock(&msc->mutex->death);
		pthread_mutex_lock(&msc->mutex->print);
		printf("%lld %d died\n", time_now, one_philo->nb_philo);
		pthread_mutex_unlock(&msc->mutex->print);
		return ;
	}
	pthread_mutex_lock(&msc->mutex->print);
	printf("%lld %d is sleeping\n", time_now, one_philo->nb_philo);
	pthread_mutex_unlock(&msc->mutex->print);
	usleep(msc->time_to_sleep * 1000);
}

/*WHAT SHOULD BE MONITORED?
- did all eat nb_must_eat - uneven, 3 sessions
- did one die
*/
