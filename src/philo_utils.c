/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 17:34:28 by mburgler          #+#    #+#             */
/*   Updated: 2023/08/01 15:51:28 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*pointer;

	pointer = malloc(nmemb * size);
	if (pointer == NULL)
		return (NULL);
	ft_bzero(pointer, nmemb * size);
	return (pointer);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	unsigned char	*temp;

	temp = s;
	i = 0;
	while (i < n)
	{
		temp[i] = '\0';
		i++;
	}
}

int	overflow_handler(const char *str, int i, int sign)
{
	int		len;
	char	*max;
	char	*min;

	max = "9223372036854775807";
	min = "9223372036854775808";
	len = 0;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		len++;
		i++;
	}
	if (len > 19)
		return (1);
	else if (len < 19)
		return (0);
	else if (sign < 0 && ft_strncmp(str + (i - len), min, 19) > 0)
		return (1);
	else if (sign > 0 && ft_strncmp(str + (i - len), max, 19) > 0)
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	nb;

	nb = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] == ' ') || (nptr[i] == '\f') || (nptr[i] == '\n') 
		|| (nptr[i] == '\r') || (nptr[i] == '\t') || (nptr[i] == '\v'))
		i++;
	if (nptr[i] == '-')
		sign = -1;
	if (nptr[i] == '+' || nptr[i] == '-')
		i++;
	if (sign > 0 && overflow_handler(nptr, i, sign))
		return (-1);
	else if (sign < 0 && overflow_handler(nptr, i, sign))
		return (0);
	while ((nptr[i] >= '0') && (nptr[i] <= '9'))
	{
		nb = (nb * 10) + (nptr[i] - 48);
		i++;
	}
	nb = (nb * sign);
	return (nb);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	long unsigned int	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] > s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		else if (s1[i] < s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
