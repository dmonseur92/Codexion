/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/22 16:36:58 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
static void	*take_dongles(void *arg)
{
	t_coder	*coder;
	long	time;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->table->dongles_mutex);

	while (!coder->left_dongle->available || !coder->right_dongle->available)
	pthread_cond_wait(&coder->table->dongles_ready, &coder->table->dongles_mutex);
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
}
