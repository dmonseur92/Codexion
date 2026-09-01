/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 16:06:14 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/01 18:26:03 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	params->burnout_time = ft_atoi(argv[2]);
	params->compile_time = ft_atoi(argv[3]);
	params->debug_time = ft_atoi(argv[4]);
	params->refactor_time = ft_atoi(argv[5]);
	params->compiles_required = ft_atoi(argv[6]);
	params->dongle_cd = ft_atoi(argv[7]);
}

static int	args_validator(int argc, char **argv)
{
	if (argc != 9)
	{
		fprintf(stderr, "Number of arguments must be exactly 8\n");
		return (0);
	}
	if (!nbr_validator(argv))
	{
		fprintf(stderr, "All arguments must be numbers except last one\n");
		fprintf(stderr, "and can't be negatives\n");
		return (0);
	}
	if (!max_int_checker(argv))
	{
		fprintf(stderr, "Numbers can 't exceed int range (max: 2147483647)\n");
		return (0);
	}
	return (1);
}

int	parser(int argc, char **argv, t_params *params)
{
	char	*schedule;

	if (args_validator(argc, argv))
	{
		schedule = schedule_parser(argv[8]);
		if ((strcmp(schedule, "invalid") == 0))
		{
			fprintf(stderr, "Last argument must be 'fifo' or 'edf'\n");
			return (0);
		}
		else
		{
			assign_params(argv, params);
			if (strcmp(schedule, "fifo") == 0)
				params->scheduler = 1;
			else if (strcmp(schedule, "edf") == 0)
				params->scheduler = 0;
			tester(params);
		}
		return (1);
	}
	else
		return (0);
}
