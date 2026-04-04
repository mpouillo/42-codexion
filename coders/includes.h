/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpouillo <mpouillo@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:40:30 by mpouillo          #+#    #+#             */
/*   Updated: 2026/04/04 14:36:51 by mpouillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define TRUE			1
# define FALSE			-1

# define COMPILING		1
# define DEBUGGING		2
# define REFACTORING	3

typedef struct s_params
{
	int		coders_nb;
	int		burnout_t;
	int		compile_t;
	int		debug_t;
	int		refactor_t;
	int		req_compiles_nb;
	int		dongle_cd;
	char	*scheduler;
}	t_params;

typedef struct s_queue
{
	pthread_mutex_t	*mutex;
	int				array[2];
	int				current_size;
}	t_queue;

typedef struct s_dongle
{
	pthread_mutex_t	*mutex;
	t_queue			*queue;
	long long		last_use;
}	t_dongle;

typedef struct s_data
{
	t_params		*params;
	pthread_mutex_t	*print_mutex;
	long long		start_time_us;
}	t_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
	long			last_compile;
	int				compile_count;
	t_data			*data;
}	t_coder;

//		handle_args.c
t_parameters	*get_parameters(char **argv);
int		validate_arguments(char **argv);

//		print.c
void	print_status(pthread_mutex_t *print_mutex, int status, int coder_id, int time);
void	print_dongle(pthread_mutex_t *print_mutex, int coder_id, int time);

//		thread_functions.c
void	*vibe_coding(void *arg);

//		timing.c
long long	get_time_since_start(void);

//		queue.c
t_queue	*q_create(void);
int		q_enqueue(t_queue *q, int coder_id);
int		q_dequeue(t_queue *q, t_data *data, t_scheduler scheduler);
void	q_destroy(t_queue *q);

#endif
