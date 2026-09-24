/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/24 17:51:10 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
static int	dongle_ready(t_dongle *dongle)
{
	return (dongle->available && get_time() >= dongle->ready_at);
}

static void	wait_until(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	struct timespec	ts;
	long			time;

	time = get_time();
	ts.tv_sec = time / 1000;
	ts.tv_nsec = (time % 1000) * 1000000;
	pthread_cond_timedwait(cond, mutex, &ts);
}

static void	*take_dongles(void *arg)
{
	t_coder	*coder;
	long	time;

	coder = (t_coder *)arg;
	if (coder->left_dongle == coder->right_dongle)
		return (lone_coder(coder));
	while (coder->compiles_required > 0)
	{
		pthread_mutex_lock(&coder->table->dongles_mutex);
		while (!coder->table->stop && (!dongle_ready(coder->left_dongle)
		|| !dongle_ready(coder->right_dongle)))
		wait_until(&coder->table->dongles_ready, &coder->table->dongles_mutex);

		if (coder->table->stop)
		{
			pthread_mutex_unlock(&coder->table->dongles_mutex);
			return (NULL);
		}
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
		coder->compiles_required --;
	}
	coder->table->stop = 1;
	return (NULL);
}

static void		*check_burnout(void *arg)
{
	t_table		*table;
	long	time;
	int		i;


	table = (t_table *)arg;
	while (!table->stop)
	{
		i = 0;
		while (i < table->params->nb_coders && !table->stop)
		{
			if (get_time() - table->coders[i]->last_compile >= table->params->burnout_time)
			{
				pthread_mutex_lock(&table->print_mutex);
				time = get_time() - table->start_time;
				printf(RED "%ld %d has burned out\n" RESET, time, table->coders[i]->coder_id);
				pthread_mutex_unlock(&table->print_mutex);
				pthread_mutex_lock(&table->dongles_mutex);
				table->stop = 1;
				pthread_cond_broadcast(&table->dongles_ready);
				pthread_mutex_unlock(&table->dongles_mutex);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	create_theards(t_table *table)
{
	pthread_t	*threads;
	pthread_t	*monitor;
	int i;

	monitor = malloc(sizeof(pthread_t));
	threads = malloc(sizeof(pthread_t) * table->params->nb_coders);
	pthread_create(monitor, NULL, check_burnout, table);
	i = 0;
	while (i < table->params->nb_coders && !table->stop)
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
	pthread_join(*monitor, NULL);
}
