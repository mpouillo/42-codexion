/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:40:30 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/01 14:51:28 by mpouillo         ###   ########.fr       */
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

typedef enum e_scheduler
{
	fifo, edf
}	t_scheduler;

typedef struct s_parameters
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongles_cooldown;
	t_scheduler	scheduler;
}	t_parameters;

typedef struct s_dongle
{
	pthread_mutex_t	*mutex;
	pthread_cond_t	*cond;
}	t_dongle;

typedef struct s_coder_data
{
	t_parameters	*parameters;
	t_dongle		*dongle;
	int				id;
	long			last_compile;
	int				nbr_compiles;
	long long		start_time_us;
}	t_coder_data;

typedef struct s_coder
{
	pthread_t		thread;
	t_coder_data	*coder_data;
	long			last_compile;
}	t_coder;

typedef struct s_data
{
	t_coder			**coders;
	t_parameters	*parameters;
	long long		start_time_us;

}	t_data;

//		handle_args.c
t_parameters	*get_parameters(char **argv);
int		validate_arguments(char **argv);

#endif
