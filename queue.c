/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 17:12:13 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/30 17:08:59 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	queue_head(t_dongle *dongle, int scheduler)
{
	int		i;
	int		best_idx;

	if (dongle->queue_size == 0)
		return (0);
	if (dongle->queue_size == 1)
		return (dongle->queue[0].coder_id);
	best_idx = 0;
	i = 1;
	while (i < dongle->queue_size)
	{
		if (req_before(&dongle->queue[i], &dongle->queue[best_idx], scheduler))
			best_idx = i;
		i++;
	}
	return (dongle->queue[best_idx].coder_id);
}

void	queue_push(t_dongle *dongle, t_coder *coder, long ticket)
{
	t_request	*req;
	long		deadline;

	deadline = coder->last_compile
		+ coder->table->params->burnout_time;
	req = &dongle->queue[dongle->queue_size];
	req->coder_id = coder->coder_id;
	req->ticket = ticket;
	req->deadline = deadline;
	dongle->queue_size++;
}

void	queue_remove(t_dongle *dongle, int coder_id)
{
	int	i;
	int	target_idx;

	target_idx = -1;
	i = 0;
	while (i < dongle->queue_size)
	{
		if (dongle->queue[i].coder_id == coder_id)
		{
			target_idx = i;
			break ;
		}
		i++;
	}
	if (target_idx != -1)
	{
		i = target_idx;
		while (i < dongle->queue_size - 1)
		{
			dongle->queue[i] = dongle->queue[i + 1];
			i++;
		}
		dongle->queue_size--;
	}
}

static int	has_priority(t_dongle *dongle, t_coder *me)
{
	int		head;

	head = queue_head(dongle, me->table->params->scheduler);
	return (head == 0 || head == me->coder_id);
}

int	queue_my_turn(t_coder *coder)
{
	return (has_priority(coder->left_dongle, coder)
		&& has_priority(coder->right_dongle, coder));
}
