/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 15:57:41 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/26 15:57:59 by dmonseur         ###   ########.fr       */
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
