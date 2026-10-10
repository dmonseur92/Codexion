/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 16:23:10 by dmonseur          #+#    #+#             */
/*   Updated: 2026/10/10 16:25:10 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_priority(t_request *req, int scheduler)
{
	if (scheduler == EDF)
		return (req->deadline);
	return (req->ticket);
}

long	first_ticket(t_coder *coder)
{
	if (coder->coder_id % 2)
		return (coder->coder_id);
	return (coder->table->params->nb_coders + coder->coder_id);
}

void	set_start(t_table *table)
{
	int	i;

	table->start_time = get_time();
	i = 0;
	while (i < table->params->nb_coders)
	{
		table->coders[i]->last_compile = table->start_time;
		queue_push(table->coders[i]->left_dongle, table->coders[i],
			first_ticket(table->coders[i]));
		queue_push(table->coders[i]->right_dongle, table->coders[i],
			first_ticket(table->coders[i]));
		i++;
	}
}

int	req_before(t_request *a, t_request *b, int scheduler)
{
	long	pa;
	long	pb;

	pa = get_priority(a, scheduler);
	pb = get_priority(b, scheduler);
	if (pa != pb)
		return (pa < pb);
	return (a->ticket < b->ticket);
}

void	print_status(t_coder *coder, char *color, char *msg, int dongle)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->print_mutex);
	if (!table->stop)
	{
		printf("%s%ld %d %s", color, get_time() - table->start_time,
			coder->coder_id, msg);
		if (dongle)
			printf(" (%d)", dongle);
		if (color[0])
			printf(RESET);
		printf("\n");
	}
	pthread_mutex_unlock(&table->print_mutex);
}
