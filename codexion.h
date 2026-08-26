/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:34:06 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/26 16:31:18 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>


typedef struct s_params
{
	int	nb_coders;
	int	burnout_t;
	int	compile_t;
	int	debug_t;
	int	refactor_t;
	int	compiles_required;
	int	dongle_cd;
	int	scheduler;
}	t_params;

// tester.c
void	tester(t_params *params);

// helpers.c
long		ft_atoi(const char *nptr);
int		ft_isnbr(char *s);
int	max_int_checker(char **argv);

// parser.c
void	parser(int argc, char **argv, t_params *params);
void	assign_params(char **argv, t_params *params);

#endif
