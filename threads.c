/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/27 16:54:30 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_until(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	struct timespec	ts;
	long			time;

	time = get_time();
	ts.tv_sec = time / 1000;
	ts.tv_nsec = (time % 1000) * 1000000;
	pthread_cond_timedwait(cond, mutex, &ts);
}

static void	*operations(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->left_dongle == coder->right_dongle)
		return (lone_coder(coder));
	while (coder->compiles_required-- > 0)
	{
		pthread_mutex_lock(&coder->table->dongles_mutex);
		queue_push(coder->left_dongle, coder, coder->table->ticket);
		queue_push(coder->right_dongle, coder, coder->table->ticket++);
		while (!coder->table->stop && (!dongle_ready(coder->left_dongle)
				|| !dongle_ready(coder->right_dongle)
				|| !queue_my_turn(coder)))
			wait_until(&coder->table->dongles_ready,
				&coder->table->dongles_mutex);
		queue_remove(coder->left_dongle, coder->coder_id);
		queue_remove(coder->right_dongle, coder->coder_id);
		if (coder->table->stop)
			return (pthread_mutex_unlock(&coder->table->dongles_mutex), NULL);
		take_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	coder->table->stop = 1;
	return (NULL);
}

static void	*burnout_loop(t_table *table)
{
	long		time;
	int			i;

	i = 0;
	while (i < table->params->nb_coders && !table->stop)
	{
		if (get_time() - table->coders[i]->last_compile
			>= table->params->burnout_time)
		{
			pthread_mutex_lock(&table->print_mutex);
			time = get_time() - table->start_time;
			printf(RED "%ld %d has burned out\n" RESET,
				time, table->coders[i]->coder_id);
			pthread_mutex_unlock(&table->print_mutex);
			pthread_mutex_lock(&table->dongles_mutex);
			table->stop = 1;
			pthread_cond_broadcast(&table->dongles_ready);
			pthread_mutex_unlock(&table->dongles_mutex);
		}
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
	pthread_create(monitor, NULL, check_burnout, table);
	i = 0;
	while (i < table->params->nb_coders && !table->stop)
	{
		pthread_create(&threads[i], NULL, operations, table->coders[i]);
		i++;
	}
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
