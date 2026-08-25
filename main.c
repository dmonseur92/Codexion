/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/25 16:06:07 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	tester(t_params *params)
{
	printf("nb_coders: %d\n", params->nb_coders);
	printf("burnout_t: %d\n", params->burnout_t);
	printf("compile_t: %d\n", params->compile_t);
	printf("debug_t: %d\n", params->debug_t);
	printf("refactor_t: %d\n", params->refactor_t);
	printf("compiles_required: %d\n", params->compiles_required);
	printf("dongle_cd: %d\n", params->dongle_cd);
	printf("scheduler: %d\n", params->scheduler);
}

int	main(int argc, char **argv)
{
	t_params	params;

	memset(&params, 0, sizeof(t_params));
	parser(argc, argv, &params);
	tester(&params);
	return (0);
}
