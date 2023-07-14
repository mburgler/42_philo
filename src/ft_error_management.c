/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_management.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 17:08:58 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/14 17:31:29 by mburgler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void    ft_error(char *str, t_msc *msc)
{
    write(2, "Error: ", 7);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	if(msc != NULL)
	{
		if(msc->mutex!= NULL)
		{
			if(msc->mutex->forks != NULL)
				free(msc->mutex->forks);
				//free if no thread
			free(msc->mutex);
		}
		if(msc->philo != NULL) //double array free
			free(msc->philo);
		free(msc);
	}
    (void)msc; //change this
    exit(1); //kill exit since not allowed
}