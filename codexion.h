/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:34:06 by dmonseur          #+#    #+#             */
/*   Updated: 2026/08/24 18:27:30 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CODEXION
# define CODEXION

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
	int scheduler;
} t_params;

// helpers.c
int	ft_atoi(const char *nptr);
int	ft_isnbr(char *s);

#endif
