/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elraira- <elraira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 16:41:47 by elraira-          #+#    #+#             */
/*   Updated: 2021/12/14 11:25:09 by elraira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/libft.h"

/*                         server.c functions                                 */

/**
 * @brief This function is called in case an error occurred in a kill() system 
 * call in server. It frees the message sent by the user and exit the program 
 * taking EXIT_FAILURE as an argument to the exit funcion since the execution 
 * was unsuccessfull.
 * @param pid the process id number
 * @param str the string that holds the user message
 */
void	ft_show_error_message_and_exit(int pid, char *str);

/**
 * @brief This function is called once the server program received full message
 * from client program to output the message. It breaks a line for better output
 * and frees the buffer that were holding the message.
 * @param message message string received from user (client program)
 * @return char* a pointer to the message
 */
char	*ft_print_message(char *message);

/**
 * @brief This is the function that will handle any signal server receives from
 * client program. 
 * 
 * SIGUSR1 is 0 and SIGUSR2 is 1.
 * 
 * The server then will recreate the string that it received bit by bit from
 * the client program.
 * 
 * @param sig signal number: SIGUSR1 is 0 and SIGUSR2 is 1.
 * @param info The siginfo_t structure is passed as the second parameter to a 
 * user signal handler function, if the SA_SIGINFO flag was specified when the 
 * handler was installed with sigaction(). 
 * @param context 
 */
void	ft_server_user_signal_handler(int sig, siginfo_t *info, void *context);

/*                       client.c functions                                   */

/**
 * @brief This function is called in case an error occurred in a kill() system 
 * call in client. It frees the message sent by the user and exit the program 
 * taking EXIT_FAILURE as an argument to the exit funcion since the execution 
 * was unsuccessfull.
 * @param str the string that holds the user message
 */
void	ft_client_error_message_and_exit(char *str);

/**
 * @brief This function is called by send_bit() when all the bits from message 
 * were received by server. It sends 8 SIGUSR1 signals representing the '\0',
 * signaling the end of the string (00000000). Once i = 8, which means all
 * characters were sent, the function returns 1.
 * @param pid process id number
 * @param str message string
 * @return returns 1 when all bits were sent and 0 if there's an error.
 */
int		ft_send_null_terminators(int pid, char *str);

/**
 * @brief This function is responsible for sending a bit from the string 
 * "message" everytime it is called. If str is not null, is it duplicated and
 * allocated using strdup in "message". The s_pid hols the process id number
 * to make sure it stays the same through the execution and when sending to
 * ft_send_null_terminator as a parameter.
 * 
 * @param pid the PID given in the main() function of client.
 * @param str the string holding the user message
 * @return int 
 */
int		ft_send_message_bit_by_bit(int pid, char *str);

/**
 * @brief this function is called every time client receives a signal - SIGUSR1 
 * or SIGUSR2 from server. If the signal received is SIGUSR1 this means that 
 * server confirms receiving a bit from client and is ready to receive another.
 * If the signal received is SIGUSR2 then it means that an error occurred in
** server causing it to quit unexpectedly.
 * @param sig signal number: SIGUSR1 is 0 and SIGUSR2 is 1.
 */
void	ft_client_user_signal_handler(int signum);

/*                       signal.h functions                                   */

/**
 * @brief initialize and empty a signal set
 * 
 * @param set pointer to the signal set
 * @return upon successful completion, sigemptyset() shall return 0; otherwise,
 * it shall return -1 and set errno to indicate the error.
 */
int		sigemptyset(sigset_t *set);

/**
 * @brief The sigaction() function allows the calling process to examine and/or
 * specify the action to be associated with a specific signal.
 * @param sig specifies the signal
 * @param act points to a structure specifying the action to be associated with 
 * the specified signal
 * @param oact the action previously associated with the signal is stored in the
 * location pointed to by the argument oact.
 * @return Upon successful completion, sigaction() shall return 0; otherwise,
 * -1 shall be returned, errno shall be set to indicate the error, and no new 
 * signal-catching function shall be installed.
 */
int		sigaction(int sig, const struct sigaction *restrict act,
			struct sigaction *restrict oact);

/**
 * @brief adds the individual signal specified by the signo to the signal set 
 * pointed to by set.
 * 
 * @param set point to a signal set
 * @param signo the individual signal that is going to be added
 * @return Upon successful completion, sigaddset() shall return 0; otherwise, 
 * it shall return -1 and set errno to indicate the error.
 */
int		sigaddset(sigset_t *set, int signo);

/**
 * @brief this function shall send a signal to a process or a group of 
 * processes specified by the pid. The signal to be sent is specified by sig 
 * and is either one from the list given in <signal.h> or 0. If sig is 0 (the 
 * ull signal), error checking is performed but no signal is actually sent. 
 * The null signal can be used to check the validity of pid.
 * 
 * @param pid process id number
 * @param sig signal
 * @return upon successful completion, 0 shall be returned. Otherwise, -1 shall 
 * be returned and errno set to indicate the error.
 */
int		kill(pid_t pid, int sig);

/**
 * @brief get identification number of the calling process
 */
pid_t	getpid(void);

#endif
