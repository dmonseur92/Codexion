/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/25 13:41:06 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int nbr_validator(char **arg)
{
	int i;

	i = 1;
	while (i < 8)
	{
		if (!ft_isnbr(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

char *schedule_parser(char *s)
{

	if ((strcmp(s, "fifo") == 0))
		return ("fifo");
	else if ((strcmp(s, "edf") == 0))
		return ("edf");
	else
		return ("invalid");
}


void	parser(int argc, char **argv, t_params *params)
{
	char *schedule;

	schedule = schedule_parser(argv[8]);
	if (argc != 9)
		fprintf(stderr, "Number of arguments must be exactly 8\n");
	else if (!nbr_validator(argv))
		fprintf(stderr, "All arguments must be number except last one\n");
	else if ((strcmp(schedule, "invalid") == 0))
		fprintf(stderr, "Last argument must be 'fifo' or 'edf'\n");
	else
	{
		params->nb_coders = ft_atoi(argv[1]);
		params->burnout_t = ft_atoi(argv[2]);
		params->compile_t = ft_atoi(argv[3]);
		params->debug_t = ft_atoi(argv[4]);
		params->refactor_t = ft_atoi(argv[5]);
		params->compiles_required = ft_atoi(argv[6]);
		params->dongle_cd = ft_atoi(argv[7]);
	}
}

int main(int argc, char **argv)
{
	t_params params;

	memset(&params, 0, sizeof(t_params));
	parser(argc, argv, &params);
	printf("%d\n", params.nb_coders);
	printf("%d\n", params.burnout_t);
	printf("%d\n", params.compile_t);
	printf("%d\n", params.debug_t);
	printf("%d\n", params.refactor_t);
	printf("%d\n", params.compiles_required);
	printf("%d\n", params.dongle_cd);
	return (0);
}
