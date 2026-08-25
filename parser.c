/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 16:06:14 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/25 16:09:04 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	nbr_validator(char **arg)
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

static char	*schedule_parser(char *s)
{
	if ((strcmp(s, "fifo") == 0))
		return ("fifo");
	else if ((strcmp(s, "edf") == 0))
		return ("edf");
	else
		return ("invalid");
}

static void	assign_params(char **argv, t_params *params)
{
	params->nb_coders = ft_atoi(argv[1]);
	params->burnout_t = ft_atoi(argv[2]);
	params->compile_t = ft_atoi(argv[3]);
	params->debug_t = ft_atoi(argv[4]);
	params->refactor_t = ft_atoi(argv[5]);
	params->compiles_required = ft_atoi(argv[6]);
	params->dongle_cd = ft_atoi(argv[7]);
}

void	parser(int argc, char **argv, t_params *params)
{
	char	*schedule;

	schedule = schedule_parser(argv[8]);
	if (argc != 9)
		fprintf(stderr, "Number of arguments must be exactly 8\n");
	else if (!nbr_validator(argv))
	{
		fprintf(stderr, "All arguments must be numbers except last one\n");
		fprintf(stderr, "and can't be negatives\n");
	}
	else if ((strcmp(schedule, "invalid") == 0))
		fprintf(stderr, "Last argument must be 'fifo' or 'edf'\n");
	else
	{
		assign_params(argv, params);
		if (strcmp(schedule, "fifo") == 0)
			params->scheduler = 1;
		else if (strcmp(schedule, "edf") == 0)
			params->scheduler = 0;
	}
}
