/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 16:20:22 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/24 18:21:28 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_params *params, t_table *table)
{
	int	i;

	table->start_time = get_time();
	pthread_cond_init(&table->dongles_ready, NULL);
	pthread_mutex_init(&table->dongles_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	table->params = params;
	table->stop = 0;
	table->ticket = 0;
	table->dongles = malloc(sizeof(t_dongle *) * params->nb_coders);
	if (!table->dongles)
		return (0);
	i = 0;
	while (i < params->nb_coders)
	{
		table->dongles[i] = malloc(sizeof(t_dongle));
		if (!table->dongles[i])
			return (0);
		table->dongles[i]->dongle_id = i + 1;
		table->dongles[i]->cooldown = params->dongle_cd;
		table->dongles[i]->available = 1;
		table->dongles[i]->ready_at = 0;
		table->dongles[i]->queue_size = 0;
		pthread_mutex_init(&table->dongles[i]->dongle_mutex, NULL);
		i++;
	}
	return (1);
}

int	init_coders(t_params *params, t_table *table)
{
	int	i;

	table->coders = malloc(sizeof(t_coder *) * params->nb_coders);
	if (!table->coders)
		return (0);
	i = 0;
	while (i < params->nb_coders)
	{
		table->coders[i] = malloc(sizeof(t_coder));
		if (!table->coders[i])
			return (0);
		table->coders[i]->coder_id = i + 1;
		table->coders[i]->last_compile = table->start_time;
		table->coders[i]->compiles_required = table->params->compiles_required;
		table->coders[i]->table = table;
		if (i == 0)
			table->coders[i]->left_dongle = table->dongles[params->nb_coders - 1];
		else
			table->coders[i]->left_dongle = table->dongles[i - 1];
		table->coders[i]->right_dongle = table->dongles[i];
		i++;
	}
	return (1);
}

