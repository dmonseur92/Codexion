/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:51:56 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/21 14:47:22 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
static void	*take_dongles(void *arg)
{
	t_coder	*coder;
	long	time;

	coder = (t_coder *)arg;
	while (1)
	{
		pthread_mutex_lock(&coder->table->dongles_mutex);
		if (coder->left_dongle->available && coder->right_dongle->available)
		{
			pthread_mutex_unlock(&coder->table->dongles_mutex);
			pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
			coder->left_dongle->available = 0;
			pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
			coder->right_dongle->available = 0;
			pthread_mutex_lock(&coder->table->print_mutex);
			time = get_time() - coder->table->start_time;
			printf("%ld %d has taken a dongle (%d)\n", time, coder->coder_id, coder->left_dongle->dongle_id);
			printf("%ld %d has taken a dongle (%d)\n", time, coder->coder_id, coder->right_dongle->dongle_id);
			pthread_mutex_unlock(&coder->table->print_mutex);
			usleep(1000000);
			coder->left_dongle->available = 1;
			coder->right_dongle->available = 1;
			pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
			pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
			break;
		}
		pthread_mutex_unlock(&coder->table->dongles_mutex);
		usleep(10000);
	}
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
