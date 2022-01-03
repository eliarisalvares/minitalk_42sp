/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 14:02:22 by elraira-          #+#    #+#             */
/*   Updated: 2021/12/14 17:03:33 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minitalk.h"

void	ft_server_error_message_and_exit(int pid, char *str)
{
	if (str)
		free(str);
	ft_putstr("An unexpected error occurred in server execution\n");
	kill(pid, SIGUSR2);
	exit(EXIT_FAILURE);
}

char	*ft_print_message(char *message)
{
	ft_putstr(message);
	ft_putchar('\n');
	free(message);
	return (NULL);
}

void	ft_server_user_signal_handler(int sig, siginfo_t *info, void *context)
{
	static char	c = 0b11111111;
	static int	bit_counter = 0;
	static int	pid = 0;
	static char	*message = 0;

	(void)context;
	if (info->si_pid)
		pid = info->si_pid;
	if (sig == SIGUSR1)
		c ^= 0b10000000 >> bit_counter;
	else if (sig == SIGUSR2)
		c |= 0b10000000 >> bit_counter;
	if (++bit_counter == 8)
	{
		if (c != '\0')
			message = ft_add_char_to_str(message, c);
		else
			message = ft_print_message(message);
		bit_counter = 0;
		c = 0b11111111;
	}
	if (kill(pid, SIGUSR1) == -1)
		ft_server_error_message_and_exit(pid, message);
}

/* 
 * The sigset_t data type is used to represent a signal set. Internally, 
 * it may be implemented as either an integer or structure type. 
*/

int	main(void)
{
	struct sigaction	sa_sig;
	sigset_t			block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sa_sig.sa_handler = 0;
	sa_sig.sa_flags = SA_SIGINFO;
	sa_sig.sa_mask = block_mask;
	sa_sig.sa_sigaction = ft_server_user_signal_handler;
	sigaction(SIGUSR1, &sa_sig, NULL);
	sigaction(SIGUSR2, &sa_sig, NULL);
	ft_putstr("Use this PID number to send your message: ");
	ft_putnbr(getpid());
	ft_putstr("\n");
	while (1)
		pause();
}
