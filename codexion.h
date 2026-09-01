/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:34:06 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/01 13:26:07 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_params
{
	int	nb_coders;
	int	burnout_time;
	int	compile_time;
	int	debug_time;
	int	refactor_time;
	int	compiles_required;
	int	dongle_cd;
	int	scheduler;
}	t_params;

typedef struct t_dongle
{
	int dongle_id;
	int cooldown;
	pthread_mutex_t dongle_mutex;
}	s_dongle;

typedef struct t_coder
{
	int	coder_id;
	int burnout_time;
	s_dongle	*left_dongle;
	s_dongle	*right_dongle;

}	s_coder;

typedef struct t_table
{
	s_coder 	**coders;
	s_dongle	**dongles;

}	s_table;

// helpers.c
long	ft_atoi(const char *nptr);
int		ft_isnbr(char *s);

// parser.c
void	parser(int argc, char **argv, t_params *params);
void	assign_params(char **argv, t_params *params);

// tester.c
void	tester(t_params *params);

// validator.c
int		max_int_checker(char **argv);
int		nbr_validator(char **arg);
#endif
