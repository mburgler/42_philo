/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:22:35 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/28 00:04:35 by mburgler         ###   ########.fr       */
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
		//printf("DEBUG NO_PHILO#, no %d\n", msc->philo[i]->nb_philo);
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
	//printf("*** BEFORE THREAD_JOIN\n");
	if(ft_pthread_join(i, philo_thread, msc) == -1) //< or <= in pthread join -> solved it is <
	 	return (ft_error("pthread_join failed", msc), -1);
	printf("*** AFTER THREAD_JOIN\n");
	free_and_nullify((void **)&philo_thread); //correct?
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
			pthread_mutex_lock(&msc->mutex->meal_count);
			if(msc->philo[i]->meal_count <= msc->nb_must_eat || msc->nb_must_eat == -1)
				all_ate = false;
			pthread_mutex_unlock(&msc->mutex->meal_count);
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
		if(philo_sleeps(one_philo, one_philo->msc) == -1)
			return (NULL);
		if(one_philo->msc->stop_simulation == false)
			ft_mutex_print(one_philo->msc, one_philo, "is thinking");
		else
			return (NULL);
	}
	//printf("DEBUG simulation_running########################\n");
	//printf("DEBUG ROUTINE#, thread_philo no %d\n", one_philo->nb_philo);
	//pthread_mutex_lock(&one_philo->msc->mutex->print);
	//printf("DEBUG ROUTINE#, thread_philo no %d\n", one_philo->nb_philo);
	//pthread_mutex_unlock(&one_philo->msc->mutex->print);
	return(NULL);
}

void	philo_eats(t_philo *one_philo, t_msc *msc)
{
	printf("*** DEBUG in func eating***\n");
	if((one_philo->nb_philo + 1) %2)
	{
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		pthread_mutex_lock(&msc->mutex->print);
		printf("%lld %d has taken his left fork\n", one_philo->time_last_meal, one_philo->nb_philo);
		pthread_mutex_unlock(&msc->mutex->print);
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		pthread_mutex_lock(&msc->mutex->print);
		printf("%lld %d has taken his right fork\n", one_philo->time_last_meal, one_philo->nb_philo);
		pthread_mutex_unlock(&msc->mutex->print);
	}
	else
	{
		usleep(1000);
		printf("###\n");
		pthread_mutex_lock(&msc->mutex->forks[*one_philo->right_fork]);
		pthread_mutex_lock(&msc->mutex->print);
		printf("%lld %d has taken his right fork\n", one_philo->time_last_meal, one_philo->nb_philo);
		pthread_mutex_unlock(&msc->mutex->print);
		pthread_mutex_lock(&msc->mutex->forks[one_philo->left_fork]);
		pthread_mutex_lock(&msc->mutex->print);
		printf("%lld %d has taken his left fork\n", one_philo->time_last_meal, one_philo->nb_philo);
		pthread_mutex_unlock(&msc->mutex->print);
	}
	printf("*** DEBUG 2 in func eating***\n");
	pthread_mutex_lock(&msc->mutex->death);
	one_philo->time_last_meal = sys_time();
	pthread_mutex_unlock(&msc->mutex->death);
	pthread_mutex_lock(&msc->mutex->print);
	printf("%lld %d is eating\n", one_philo->time_last_meal, one_philo->nb_philo);
	pthread_mutex_unlock(&msc->mutex->print);
	pthread_mutex_lock(&msc->mutex->meal_count);
	one_philo->meal_count++;
	pthread_mutex_unlock(&msc->mutex->meal_count);
	usleep(msc->time_to_eat * 1000);
	printf("DONE EATING\n");
	pthread_mutex_unlock(&msc->mutex->forks[one_philo->left_fork]);
	pthread_mutex_unlock(&msc->mutex->forks[*one_philo->right_fork]);
}

int	philo_sleeps(t_philo *one_philo, t_msc *msc)
{
	long long time_now;

	if(msc->stop_simulation == true)
		return (-1);
	time_now = sys_time();
	printf("**** TIME LAST MEAL %lld, TIME TO DIE %d", (time_now - one_philo->time_last_meal), msc->time_to_die);
	if(time_now - one_philo->time_last_meal > msc->time_to_die)
	{
		pthread_mutex_lock(&msc->mutex->death);
		one_philo->dead = true;
		pthread_mutex_unlock(&msc->mutex->death);
		msc->stop_simulation = true;
		ft_mutex_print(msc, one_philo, "died");		
		return (-1);
	}
	ft_mutex_print(msc, one_philo, "is sleeping");
	usleep(msc->time_to_sleep * 1000);
	return(0);
}

/*WHAT SHOULD BE MONITORED?
- did all eat nb_must_eat - uneven, 3 sessions
- did one die
*/