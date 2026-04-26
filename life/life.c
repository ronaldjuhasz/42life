#include "life.h"

/* Parse input commands, evolve the board, and print the result. */
int main (int ac, char** av)
{
	int rb;
	char buff;
	t_life s = {0};

	if (ac != 4)
	{
		print_error(NULL, BA);
		return (EXIT_FAILURE);
	}
	if (!init_table(&s, av))
	{
		print_error(&s, IF);
		return (EXIT_FAILURE);
	}
	
	do
	{
		rb = read(0, &buff, 1);
		if (rb < 0)
		{
			print_error(&s, RE);
			return (EXIT_FAILURE);
		}
		get_pos(&s, buff);
	}
	while (rb);

	if (s.iterations)
	{
		if (!life(&s))
			return (EXIT_FAILURE);
	}
	pr_table(&s);
	return (EXIT_SUCCESS);
}

/* Print an error message and release any allocated state. */
void print_error(t_life* s, char* err_mess)
{
	int i;

	i = 0;
	while (err_mess[i])
		putchar(err_mess[i++]);
	putchar('\n');
	if (s)
		free_struct(s);
}


/* Read dimensions, allocate the board, and initialize pen state. */
bool init_table(t_life* s, char* av[])
{
	s->width = atoi(av[1]);
	s->height = atoi(av[2]);
	s->iterations = atoi(av[3]);
	if (s->width < 0  || s->height < 0 || s->iterations < 0)
		return (false);
	s->table = make_table(s);
	if (!s->table)
	{
		print_error(s, MF);
		return (false);
	}
	s->pen = calloc(1, sizeof(t_pen));
	if (!s->pen)
	{
		return (false);
	}
	s->pen->position[0] = 0;
	s->pen->position[1] = 0;
	s->pen->x_position[0] = -1;
	s->pen->x_position[1] = -1;
	s->pen->down = false;
	return (true);
}

/* Allocate a fresh board and initialize every cell as dead. */
char** make_table(t_life* s)
{
	char** table;
	table = malloc(s->height * sizeof(char*));
	if (!table)
	{
		return (NULL);
	}
	for (int i = 0; i < s->height; i++)
	{
		table[i] = malloc(s->width  * sizeof(char));
		if (!table[i])
		{
			while (i--)
				free (table[i]);
			free (table);
			return (NULL);
		}
	}
	for (int i = 0; i < s->height; i++)
	{
		for (int j = 0; j < s->width; j++)
			table[i][j] = DEAD;
	}
	return (table);
}

/* Release every row in the board and then the board pointer itself. */
void free_table(t_life* s)
{
	if (s->table)
	{
		for (int i = 0; i < s->height; i++)
		{
			if (s->table[i])
				free(s->table[i]);
		}
		free(s->table);
	}
	s->table = NULL;
}

/* Free the board and pen objects owned by the simulation. */
void free_struct(t_life* s)
{
	if (s->table)
	{
		free_table(s);
	}
	if (s->pen)
	{
		free(s->pen);
	}
	s->pen = NULL;
}

/* Check whether a coordinate is inside the current board. */
bool in_table(t_life* s, int x, int y)
{
	return (y >= 0 && y < s->height && x >= 0 && x < s->width);
}

/* Move the pen and mark cells when drawing mode is enabled. */
void update_set(t_life* s, int y, int x)
{
	s->pen->position[0] = y;
	s->pen->position[1] = x;
	if (s->pen->down)
		s->table[y][x] = ALIVE;		
	if (s->pen->x_position[0] > -1 && s->pen->x_position[1] > -1 && s->pen->down)
	{
		s->table[s->pen->x_position[0]][s->pen->x_position[1]] = ALIVE;
		s->pen->x_position[0] = -1;
		s->pen->x_position[1] = -1;
	}
}

/* Handle a single input command and update the pen state accordingly. */
void get_pos(t_life* s, char c)
{
	int x = s->pen->position[1];
	int y = s->pen->position[0];

	switch (c)
	{
		case 'x':
		s->pen->down = !s->pen->down;
		if (s->pen->down)
		{
			s->pen->x_position[0] = y;
			s->pen->x_position[1] = x;
		}		
		break; 
		case 'w':
		if (in_table(s, x, y - 1))
			y--;
		break;
		case 's':
		if (in_table(s, x, y + 1))
			y++;
		break;
		case 'd':
		if (in_table(s, x + 1, y))
			x++;
		break;
		case 'a':
		if (in_table(s, x - 1, y))
			x--;
		break;
		default:
		break;
	}

	if (x != s->pen->position[1] || y != s->pen->position[0])
	{
		update_set(s, y, x);
	}	
}

/* Count live neighbors for the Game of Life rules. */
int neighbour(t_life* s, int i, int j)
{
	int count;
	
	count = 0;
	
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if ((!(!x && !y )) && in_table(s, (j + x), (i + y)) && s->table[i + y][j + x] == ALIVE)
				count++;
		}
	}
	return (count);
	
}

/* Apply one simulation step for the requested number of iterations. */
bool life(t_life* s)
{
	while (s->iterations--)
	{
		/* Build the next generation from the current board state. */
		char** new_table = make_table(s);
		if (!new_table)
		{
			print_error(s, MF);
			return (false);
		}
		for (int i = 0; i < s->height; i++)
		{
			for (int j = 0; j < s->width; j++)
			{
				int n = neighbour(s, i, j);
				if (s->table[i][j] == ALIVE)
				{
					if (n < 2)								//underpopulation
							new_table[i][j] = DEAD;
					else if (n <= 3)
							new_table[i][j] = ALIVE;
					else if (n > 3)							//overpopulation
							new_table[i][j] = DEAD;				
				}			 
				else if (s->table[i][j] == DEAD && n == 3) //reproduction
					new_table[i][j] = ALIVE;
			}
		}
		free_table(s);
		s->table = new_table;
	}
	return (true);
}

/* Print the final board and clean up the simulation state. */
void pr_table(t_life* s)
{
	for (int i = 0; i < s->height; i++)
	{
		for (int j = 0; j < s->width; j++)
			putchar(s->table[i][j]);
		putchar('\n');
	}
	free_struct(s);
}
