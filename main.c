/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/22 19:12:27 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params	params;
	t_table		table;

	memset(&params, 0, sizeof(t_params));
	if (parser(argc, argv, &params))
	{
		init_dongles(&params, &table);
		init_coders(&params, &table);
		create_theards(&table);
	}
	return (0);
}
