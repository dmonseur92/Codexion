/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:49:28 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/22 19:37:06 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	long	time;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(GREEN "%ld %d is compiling\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->compile_time *1000);
	coder->last_compile = get_time();
}

void debug(t_coder *coder)
{
	long	time;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(YELLOW "%ld %d is debugging\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->debug_time * 1000);
}

void refactor(t_coder *coder)
{
	long	time;
	pthread_mutex_lock(&coder->table->print_mutex);
	time = get_time() - coder->table->start_time;
	printf(BLUE "%ld %d is refactoring\n" RESET, time, coder->coder_id);
	pthread_mutex_unlock(&coder->table->print_mutex);
	usleep(coder->table->params->refactor_time * 1000);
}
