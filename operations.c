/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:49:28 by dmonseur          #+#    #+#             */
/*   Updated: 2026/10/01 13:13:19 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	pthread_mutex_unlock(&coder->table->dongles_mutex);
	print_status(coder, "", "has taken a dongle",
		coder->left_dongle->dongle_id);
	print_status(coder, "", "has taken a dongle",
		coder->right_dongle->dongle_id);
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
	if (coder->table->stop)
		return ;
	coder->last_compile = get_time();
	print_status(coder, GREEN, "is compiling", 0);
	usleep(coder->table->params->compile_time * 1000);
}

void	debug(t_coder *coder)
{
	if (coder->table->stop)
		return ;
	print_status(coder, YELLOW, "is debugging", 0);
	usleep(coder->table->params->debug_time * 1000);
}

void	refactor(t_coder *coder)
{
	if (coder->table->stop)
		return ;
	print_status(coder, BLUE, "is refactoring", 0);
	usleep(coder->table->params->refactor_time * 1000);
}

void	*lone_coder(t_coder *coder)
{
	print_status(coder, "", "has taken a dongle",
		coder->left_dongle->dongle_id);
	return (NULL);
}
