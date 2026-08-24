/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/24 18:53:05 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	parser(int argc, char **argv, t_params *params)
{
	int *field[7];
	int i;

	if (argc != 9)
		fprintf(stderr, "The program must take exactly 8 arguments");
	field[0] = &params->nb_coders;
	field[1] = &params->burnout_t;
	field[2] = &params->compile_t;
	field[3] = &params->debug_t;
	field[4] = &params->refactor_t;
	field[5] = &params->compiles_required;
	field[6] = &params->dongle_cd;
	i = 0;
	while (i < 7)
	{
		if (ft_isnbr(argv[i + 1]))
			*field[i] = ft_atoi(argv[i + 1]);
		else
			fprintf(stderr, "All arguments must be number execpt last one");
		i++;
	}
	i = 0;
	while (i < 8)
	{
		printf("%d\n", *field[i]);
		i++;
	}
}



int main(int argc, char **argv)
{
	t_params params;

	memset(&params, 0, sizeof(t_params));
	parser(argc, argv, &params);
	return (0);
}
