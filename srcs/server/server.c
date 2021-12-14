#include "../../includes/minitalk.h"

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
	static int	bits = 0;
	static int	pid = 0;
	static char	*message = 0;

	(void)context;
	if (info->si_pid)
		pid = info->si_pid;
	if (sig == SIGUSR1)
		c ^= 0b10000000 >> bits;
	else if (sig == SIGUSR2)
		c |= 0b10000000 >> bits;
	if (++bits == 8)
	{
		if (c != '\0')
			message = ft_straddc(message, c);
		else
			message = ft_print_message(message);
		bits = 0;
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