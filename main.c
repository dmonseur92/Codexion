/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/27 17:04:33 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_up(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->params->nb_coders)
	{
		free(table->dongles[i]);
		free(table->coders[i]);
		i++;
	}
	free(table->dongles);
	free(table->coders);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_table		table;

	memset(&params, 0, sizeof(t_params));
	if (parser(argc, argv, &params))
	{
		if (init_dongles(&params, &table)
			&& init_coders(&params, &table))
			create_theards(&table);
		clean_up(&table);
	}
	return (0);
}
