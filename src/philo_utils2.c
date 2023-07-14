/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburgler <mburgler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:26:00 by mburgler          #+#    #+#             */
/*   Updated: 2023/07/14 19:03:22 by mburgler         ###   ########.fr       */
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