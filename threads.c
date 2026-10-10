/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/10/10 16:34:55 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*operations(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->table->dongles_mutex);
	pthread_mutex_unlock(&coder->table->dongles_mutex);
	if (coder->left_dongle == coder->right_dongle)
		return (lone_coder(coder));
	while (coder->compiles_required-- > 0)
	{
		if (!wait_for_dongles(coder))
			return (NULL);
		take_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	coder->has_finished = 1;
	stop_program(coder);
	return (NULL);
}

static void	*burnout_loop(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->params->nb_coders && !table->stop)
	{
		if (!table->coders[i]->has_finished
			&& get_time() - table->coders[i]->last_compile
			>= table->params->burnout_time)
			declare_burnout(table, table->coders[i]->coder_id);
		i++;
	}
	usleep(1000);
	return (NULL);
}

static void	*check_burnout(void *arg)
{
	t_table		*table;

	table = (t_table *)arg;
	while (!table->stop)
		burnout_loop(table);
	return (NULL);
}

void	create_theards(t_table *table)
{
	pthread_t	*threads;
	pthread_t	*monitor;
	int			i;

	monitor = malloc(sizeof(pthread_t));
	threads = malloc(sizeof(pthread_t) * table->params->nb_coders);
	pthread_mutex_lock(&table->dongles_mutex);
	i = 0;
	while (i < table->params->nb_coders)
	{
		pthread_create(&threads[i], NULL, operations, table->coders[i]);
		i++;
	}
	set_start(table);
	pthread_create(monitor, NULL, check_burnout, table);
	pthread_mutex_unlock(&table->dongles_mutex);
	i = 0;
	while (i < table->params->nb_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(*monitor, NULL);
	free(threads);
	free(monitor);
}
