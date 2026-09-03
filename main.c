/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:30:24 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/03 17:18:10 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_table(t_params *params, t_table *table)
{
	int	i;

	i = 0;
	while (i < params->nb_coders)
	{
		printf("\n=== Coder %d ===\n", table->coders[i]->coder_id);
		printf("Burnout time : %d\n", table->coders[i]->burnout_time);
		printf("Left dongle  : %d\n",
			table->coders[i]->left_dongle->dongle_id);
		printf("Right dongle : %d\n",
			table->coders[i]->right_dongle->dongle_id);
		printf("Left cooldown  : %d\n",
			table->coders[i]->left_dongle->cooldown);
		printf("Right cooldown : %d\n",
			table->coders[i]->right_dongle->cooldown);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_table		table;

	memset(&params, 0, sizeof(t_params));
	if (parser(argc, argv, &params))
	{
		init_dongles(&params, &table);
		init_coders(&params, &table);
		// print_table(&params, &table);
		create_theards(&table);
	}
	return (0);
}
