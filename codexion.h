/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmonseur <dmonseur@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:34:06 by dmonseur          #+#    #+#             */
/*   Updated: 2026/09/22 17:37:35 by dmonseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_table t_table;

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

typedef struct s_dongle
{
	int				dongle_id;
	int				cooldown;
	int				available;
	long			ready_at;
	pthread_mutex_t	dongle_mutex;
}	t_dongle;

typedef struct s_coder
{
	int			coder_id;
	int			burnout_time;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_table		*table;
}	t_coder;

typedef struct s_table
{
	t_params	*params;
	t_coder		**coders;
	t_dongle	**dongles;
	long		start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dongles_mutex;
	pthread_cond_t	dongles_ready;
}	t_table;

// init.c
int		init_dongles(t_params *params, t_table *table);
int		init_coders(t_params *params, t_table *table);

// operations.c
void	compile(t_coder *coder);
void 	debug(t_coder *coder);
void 	refactor(t_coder *coder);
// parser.c
int		parser(int argc, char **argv, t_params *params);

// tester.c
void	tester(t_params *params);

// threads.c
void	create_theards(t_table *table);

// utils.c
long	ft_atoi(const char *nptr);
int		ft_isnbr(char *s);
long	get_time();

// validator.c
int		max_int_checker(char **argv);
int		nbr_validator(char **arg);
#endif
