/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 19:32:58 by dmonseur          #+#    #+#             */
/*   Updated: 2026/10/01 12:28:27 by dmonseur         ###   ########.fr       */
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

void	stop_program(t_coder *coder)
{
	int		i;

	i = 0;
	while (i < coder->table->params->nb_coders)
	{
		if (!coder->table->coders[i]->has_finished)
			return ;
		i++;
	}
	coder->table->stop = 1;
}
