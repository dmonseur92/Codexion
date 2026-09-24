/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 17:12:13 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/24 18:13:53 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include "codexion.h"

static int	queue_head(t_dongle *dongle, int scheduler)
{
	t_request	*a;
	t_request	*b;

	if (dongle->queue_size < 2)
		return (dongle->queue_size * dongle->queue[0].coder_id);
	a = &dongle->queue[0];
	b = &dongle->queue[1];
	if (scheduler == EDF && a->deadline != b->deadline)
	{
		if (a->deadline < b->deadline)
			return (a->coder_id);
		return (b->coder_id);
	}
	if (a->ticket < b->ticket)
		return (a->coder_id);
	return (b->coder_id);
}

void	queue_push(t_dongle *dongle, t_coder *coder, long ticket)
{
	t_request	*req;
	
	long		deadline;

	deadline = coder->last_compile + coder->table->params->burnout_time;
	req = &dongle->queue[dongle->queue_size];
	req->coder_id = coder->coder_id;
	req->ticket = ticket;
	req->deadline = deadline;
	dongle->queue_size++;
}

void	queue_remove(t_dongle *dongle, int coder_id)
{
	if (dongle->queue_size > 0 && dongle->queue[0].coder_id == coder_id)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue_size--;
	}
	else if (dongle->queue_size > 1 && dongle->queue[1].coder_id == coder_id)
		dongle->queue_size--;
}

static int	has_priority(t_dongle *dongle, t_coder *me)
{
	int		head;
	t_coder	*other;

	head = queue_head(dongle, me->table->params->scheduler);
	if (head == me->coder_id)
		return (1);
	other = me->table->coders[head - 1];
	return (!dongle_ready(other->left_dongle)
		|| !dongle_ready(other->right_dongle));
}

int	queue_my_turn(t_coder *coder)
{
	return (has_priority(coder->left_dongle, coder)
		&& has_priority(coder->right_dongle, coder));
}

