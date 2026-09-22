/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/22 17:38:44 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
static int	dongle_ready(t_dongle *d)
{
	return (d->available && get_time() >= d->ready_at);
}

static void	wait_next_check(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	struct timeval	tv;
	struct timespec	ts;
	long			ns;

	gettimeofday(&tv, NULL);
	ns = (tv.tv_usec * 1000) + 2000000L;
	ts.tv_sec = tv.tv_sec + (ns / 1000000000L);
	ts.tv_nsec = ns % 1000000000L;
	pthread_cond_timedwait(cond, mutex, &ts);
}

static void	*take_dongles(void *arg)
{
	t_coder	*coder;
	long	time;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->table->dongles_mutex);
	while (!dongle_ready(coder->left_dongle) || !dongle_ready(coder->right_dongle))
		wait_next_check(&coder->table->dongles_ready, &coder->table->dongles_mutex);
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	pthread_mutex_unlock(&coder->table->dongles_mutex);

	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf("%ld %d has taken a dongle (%d)\n", time, coder->coder_id, coder->left_dongle->dongle_id);
	printf("%ld %d has taken a dongle (%d)\n", time, coder->coder_id, coder->right_dongle->dongle_id);
	pthread_mutex_unlock(&coder->table->print_mutex);

	compile(coder);

	pthread_mutex_lock(&coder->table->dongles_mutex);
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->left_dongle->ready_at = get_time() + coder->left_dongle->cooldown;
	coder->right_dongle->ready_at = get_time() + coder->right_dongle->cooldown;
	pthread_cond_broadcast(&coder->table->dongles_ready);
	pthread_mutex_unlock(&coder->table->dongles_mutex);

	debug(coder);
	refactor(coder);
	return (NULL);
}


void	create_theards(t_table *table)
{
	pthread_t	*threads;
	int i;


	threads = malloc(sizeof(pthread_t) * table->params->nb_coders);
	while (table->params->compiles_required > 0)
	{
		i = 0;
		while (i < table->params->nb_coders)
		{
			pthread_create(&threads[i], NULL, take_dongles, table->coders[i]);
			i++;
		}
		i = 0;
		while (i < table->params->nb_coders)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
		table->params->compiles_required --;
	}
}
