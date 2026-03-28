/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:40:30 by mpouillo          #+#    #+#             */
/*   Updated: 2026/03/28 14:46:56 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

typedef struct s_variables
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongles_cooldown;
	char	*scheduler;
}	t_variables;

typedef struct s_dongle
{
	pthread_mutex_t	*dongle_mutexes;
	pthread_cond_t	condition_cond;
}	t_dongle;

typedef struct s_coder
{
	pthread_t	thread;
	int			id;
	long		last_compile;
}	t_coder;

typedef struct s_data
{
	t_coder		**coders;
	t_dongle	*dongles;
	t_variables	*variables;

}	t_data;

//		handle_args.c
void	fill_variables(t_variables *variables, char **argv);
int		validate_arguments(char **argv);

#endif
