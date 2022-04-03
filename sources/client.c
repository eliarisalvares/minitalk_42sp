/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elraira- <elraira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 14:02:22 by elraira-          #+#    #+#             */
/*   Updated: 2022/04/03 09:16:41 by elraira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minitalk.h"

void	ft_client_error_message_and_exit(char *str)
{
	if (str)
		free(str);
	ft_putstr("An unexpected error occurred in client execution\n");
	exit(EXIT_FAILURE);
}

int	ft_send_null_terminators(int pid, char *str)
{
	static int	i = 0;

	if (i++ != 8)
	{
		if (kill(pid, SIGUSR1) == -1)
			ft_client_error_message_and_exit(str);
		return (0);
	}
	return (1);
}

int	ft_send_message_bit_by_bit(int pid, char *str)
{
	static char	*message = 0;
	static int	s_pid = 0;
	static int	bit_counter = -1;

	if (str)
		message = ft_strdup(str);
	if (!message)
		ft_client_error_message_and_exit(0);
	if (pid)
		s_pid = pid;
	if (message[++bit_counter / 8])
	{
		if (message[bit_counter / 8] & (0b10000000 >> (bit_counter % 8)))
		{
			if (kill(s_pid, SIGUSR2) == -1)
				ft_client_error_message_and_exit(message);
		}
		else if (kill(s_pid, SIGUSR1) == -1)
			ft_client_error_message_and_exit(message);
		return (0);
	}
	if (!ft_send_null_terminators(s_pid, message))
		return (0);
	free(message);
	return (1);
}

void	ft_client_user_signal_handler(int sig)
{
	int	end;

	end = 0;
	if (sig == SIGUSR1)
		end = ft_send_message_bit_by_bit(0, 0);
	else if (sig == SIGUSR2)
	{
		ft_putstr("An error caused server to end unexpectedly.\n");
		exit(EXIT_FAILURE);
	}
	if (end)
	{
		ft_putstr("Message was successfully sent to server!\n");
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv)
{
	if (argc != 3 || !ft_str_isdigit(argv[1]))
	{
		ft_putstr("Invalid arguments.\n");
		ft_putstr("Try: ./client + server PID number + message.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		signal(SIGUSR1, ft_client_user_signal_handler);
		signal(SIGUSR2, ft_client_user_signal_handler);
		ft_send_message_bit_by_bit(atoi(argv[1]), argv[2]);
	}
	while (1)
		pause();
}
