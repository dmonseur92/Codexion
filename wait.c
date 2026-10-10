/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 16:35:05 by dmonseur          #+#    #+#             */
/*   Updated: 2026/10/10 16:35:06 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_until(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	struct timespec	ts;
	long			time;

	time = get_time() + 1;
	ts.tv_sec = time / 1000;
	ts.tv_nsec = (time % 1000) * 1000000;
	pthread_cond_timedwait(cond, mutex, &ts);
}

static void	request_dongles(t_coder *coder)
{
	long	ticket;

	if (coder->compiles_required
		>= coder->table->params->compiles_required - 1)
		return ;
	ticket = coder->table->ticket++;
	queue_push(coder->left_dongle, coder, ticket);
	queue_push(coder->right_dongle, coder, ticket);
}

int	wait_for_dongles(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->dongles_mutex);
	request_dongles(coder);
	while (!table->stop && (!dongle_ready(coder->left_dongle)
			|| !dongle_ready(coder->right_dongle) || !queue_my_turn(coder)))
		wait_until(&table->dongles_ready, &table->dongles_mutex);
	queue_remove(coder->left_dongle, coder->coder_id);
	queue_remove(coder->right_dongle, coder->coder_id);
	if (table->stop)
	{
		pthread_mutex_unlock(&table->dongles_mutex);
		return (0);
	}
	return (1);
}
