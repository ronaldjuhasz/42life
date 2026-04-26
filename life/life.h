#ifndef LIFE_H
# define LIFE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>

/* Error messages */
# define BA "Error: Bad argument. Try: ./life width height iterations"
# define IF "Error: Initialisation failed."
# define RE "Error: Read failed."
# define MF "Error: Memmory allocation failed."

/* Cell states */
# define ALIVE 'O'
# define DEAD ' '

/* Drawing state while parsing input commands */
typedef struct s_pen
{
	int		position[2];
	bool	down;
	int		x_position[2];
} t_pen;

/* Whole simulation state */
typedef struct s_life
{
	int		width;
	int		height;
	int		iterations;
	char	**table;
	t_pen	*pen;
} t_life;

/* Error handling and cleanup */
void print_error(t_life* s, char* err_mess);
void free_table(t_life* s);
void free_struct(t_life* s);

/* Input handling and board editing */
bool in_table(t_life* s, int x, int y);
void update_set(t_life* s, int y, int x);
void get_pos(t_life* s, char c);

/* Simulation lifecycle */
char** make_table(t_life* s);
bool init_table(t_life* s, char* av[]);
int neighbour(t_life* s, int i, int j);
bool life(t_life* s);

/* Output */
void pr_table(t_life* s);

#endif