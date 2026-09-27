/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:49:28 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/27 16:10:14 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	long	time;

	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	pthread_mutex_unlock(&coder->table->dongles_mutex);
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf("%ld %d has taken a dongle (%d)\n",
		time, coder->coder_id, coder->left_dongle->dongle_id);
	printf("%ld %d has taken a dongle (%d)\n",
		time, coder->coder_id, coder->right_dongle->dongle_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	compile(coder);
	pthread_mutex_lock(&coder->table->dongles_mutex);
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	coder->left_dongle->ready_at = get_time() + coder->left_dongle->cooldown;
	coder->right_dongle->ready_at = get_time() + coder->right_dongle->cooldown;
	pthread_cond_broadcast(&coder->table->dongles_ready);
	pthread_mutex_unlock(&coder->table->dongles_mutex);
}

void	compile(t_coder *coder)
{
	long	time;

	if (coder->table->stop)
		return ;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(GREEN "%ld %d is compiling\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->compile_time * 1000);
	coder->last_compile = get_time();
}

void	debug(t_coder *coder)
{
	long	time;

	if (coder->table->stop)
		return ;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(YELLOW "%ld %d is debugging\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->debug_time * 1000);
}

void	refactor(t_coder *coder)
{
	long	time;

	if (coder->table->stop)
		return ;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(BLUE "%ld %d is refactoring\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->refactor_time * 1000);
}

void	*lone_coder(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf("%ld %d has taken a dongle (%d)\n", time,
		coder->coder_id, coder->left_dongle->dongle_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	return (NULL);
}
