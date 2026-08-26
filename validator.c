/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 19:32:58 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/26 19:34:06 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	max_int_checker(char **argv)
{
	int		i;
	long	n;

	i = 1;
	while (i < 8)
	{
		n = ft_atoi(argv[i]);
		if (n > 2147483647)
			return (0);
		i++;
	}
	return (1);
}

int	nbr_validator(char **arg)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!ft_isnbr(arg[i]))
			return (0);
		i++;
	}
	return (1);
}
