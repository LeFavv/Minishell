/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:47:08 by kevwang           #+#    #+#             */
/*   Updated: 2025/04/26 10:47:09 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_len_malloc(int n)
{
	int	len;

	len = 1;
	if (n == -2147483648)
		return (10);
	if (n < 0)
		n = -n;
	while (n > 9)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_new_str(int n, int len_n, int signe)
{
	int		i;
	char	*newstr;
	int		abs;

	newstr = malloc(sizeof(char) * (len_n + signe) + 1);
	if (newstr == NULL)
		return (NULL);
	i = 0;
	if (signe == 1)
		newstr[0] = '-';
	while (i < len_n)
	{
		abs = n % 10;
		if (n < 0)
			abs = abs * -1;
		newstr[len_n + signe - i - 1] = abs + '0';
		n = n / 10;
		i++;
	}
	newstr[len_n + signe] = '\0';
	return (newstr);
}

char	*ft_itoa(int n)
{
	int	len_n;
	int	signe;

	signe = 0;
	len_n = ft_len_malloc(n);
	if (n < 0)
		signe = 1;
	return (ft_new_str(n, len_n, signe));
}
/*
int main()
{
	char *res = ft_itoa(9);
	printf("%s\n", res);
	free(res);
	printf("%s\n", ft_itoa(1));
	printf("%s\n", ft_itoa(10));
	printf("%s\n", ft_itoa(100));
	printf("%s\n", ft_itoa(-10));
	printf("%s\n", ft_itoa(-1));
	// printf("%s\n", ft_itoa(-2147483648));

	printf("%s\n", ft_itoa(214));
	printf("%s\n", ft_itoa(2147483647));

	printf("%s\n", ft_itoa(5154644));
	printf("%s\n", ft_itoa(-0));

	// printf("%s\n", ft_itoa(2147483648));
	printf("%s\n", ft_itoa(-1));

	// printf("%d\n", 21 % 10);
	// printf("%d\n", (-21) % 10);

	// (n = -n) a faire sinon par exemple : 
	//  21 % 10 = 1  /  1 + '0' = '1'  (ascii : 49)
	// -21 % 10 = -1 / -1 + '0' = '/'  (ascii : 47)
}
*/