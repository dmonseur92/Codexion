/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 16:23:10 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/30 16:23:43 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_priority(t_request *req, int scheduler)
{
	if (scheduler == EDF)
		return (req->deadline);
	return (req->ticket);
}
