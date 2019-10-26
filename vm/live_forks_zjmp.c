/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshellie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 12:48:37 by lshellie          #+#    #+#             */
/*   Updated: 2019/10/20 12:48:40 by lshellie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../corewar.h"

int 	g_live[6] = {1, 0, T_DIR, 0, 0, 4};
int 	g_fork[6] = {1, 0, T_DIR, 0, 0, 2};

static int		get_dir(t_main *m, t_cursor *c, int pos, int size)
{
	char	dir[4];
	int		i;
	int		t_dir;

	i = -1;
	t_dir = 0;
	while (++i < 4)
		dir[i] = 0;
	i = -1;
	while (++i < size)
		dir[size - i - 1] = m->field[c_p(c->pos + pos + i)];
	if (size == 4)
		t_dir = (*((int *)dir));
	else if (size == 2)
		t_dir = (*((short *)dir));
	return (t_dir);
}

void		do_live(t_main *m, t_cursor *c)
{
	char	dir[4];
	int		i;
	int		t_dir;

	i = -1;
	while (++i < g_live[T_DIR_SIZE])
		dir[3 - i] = m->field[c_p(c->pos + 1 + i)];
	t_dir = *((int *)dir);
	if (m->v_flag)
		printf("P\t%d | live %d\n", c->id + 1, t_dir);
	t_dir = ft_abs(t_dir);
	++m->live_num;
	c->last_live_cycle = m->total_cycle;
	if (t_dir > 0 && t_dir <= m->num_of_players)
		m->last_player_live = t_dir;
	c->op = 0;
	c->pos = c_p(c->pos + g_live[T_DIR_SIZE] + 1);
}

void		do_zjmp(t_main *m, t_cursor *c)
{
	int		t_dir;

	t_dir = get_dir(m, c, 1, 2);
	if (c->carry)
	{
		if (t_dir)
			c->pos = c_p(c->pos + t_dir % IDX_MOD);
		if (m->v_flag)
			printf("P\t%d | zjmp %d OK\n", c->id + 1, t_dir);
	}
	else
	{
		c->pos = c_p(c->pos + 3);
		if (m->v_flag)
			printf("P\t%d | zjmp FaIl\n", c->id + 1);
	}
	c->op = 0;
}

void		do_fork(t_main *m, t_cursor *c)
{
	int 		dir;
	t_cursor	*new;
	int 		i;

	i = -1;
	if (c->op == FORK)
	{
		dir = get_dir(m, c, 1, 2) % IDX_MOD;
		if (m->v_flag)
			printf("P\t%d | fork %d\n", c->id + 1, dir);
	}
	else if (c->op == LFORK)
	{
		dir = get_dir(m, c, 1, 2);
		if (m->v_flag)
			printf("P\t%d | lfork %d\n", c->id + 1, dir);
	}
	new = new_cursor();
	new->pos = c_p(c->pos + dir);
	new->last_live_cycle = c->last_live_cycle;
	while (++i < 16)
		new->reg[i] = c->reg[i];
	new->id = ++m->cursor_ids;
	new->carry = c->carry;
	set_cursor(&m->cursor, new);
	c->pos = c_p(c->pos + 2 + 1);
	c->op = 0;
}
