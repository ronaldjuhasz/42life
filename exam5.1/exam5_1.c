#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ALIVE 'O'
#define DEAD ' '

typedef struct s
{
	int w;		//width of the table
	int h;		//height of the table
	int i;		//iterations		
	char** map;	//table
	
	int pp[2];	//pen's position
	bool on;	//pen's status
	int xp[2];	//pen's position when 'x' was pushed
} t;

bool in_map(t *s, int x, int y)
{
	return (y >= 0 && y < s->h && x >= 0 && x < s->w);
}

void get_pos(char c, t* s)
{
	int x = s->pp[1];
	int y = s->pp[0];

	switch (c)
	{
		case 'x':
		s->on = !s->on;
		if (s->on)
		{
			s->xp[0] = y;
			s->xp[1] = x;
		}		
		break; 
		case 'w':
		if (in_map(s, x, y - 1))
			y--;
		break;
		case 's':
		if (in_map(s, x, y + 1))
			y++;
		break;
		case 'd':
		if (in_map(s, x + 1, y))
			x++;
		break;
		case 'a':
		if (in_map(s, x - 1, y))
			x--;
		break;
		default:
		break;
	}

	if (x != s->pp[1] || y != s->pp[0])
	{
		s->pp[0] = y;
		s->pp[1] = x;
		if (s->on)
			s->map[y][x] = ALIVE;		
		if (s->xp[0] > -1 && s->xp[1] > -1 && s->on)
		{
			s->map[s->xp[0]][s->xp[1]] = ALIVE;
			s->xp[0] = -1;
			s->xp[1] = -1;
		}
	}	
}
	
char** make_map(t* s)
{
	char** map;
	map = malloc(s->h * sizeof(char*));
	for (int i = 0; i < s->h; i++)
		map[i] = malloc((s->w + 1) * sizeof(char));
	for (int i = 0; i < s->h; i++)
	{
		for (int j = 0; j < s->w; j++)
			map[i][j] = DEAD;
		map[i][s->w] = '\0';
	}
	return (map);
}
void free_map(t* s)
{
	for (int i = 0; i < s->h; i++)
		free(s->map[i]);
	free(s->map);
}
void pr_map(t* s)
{
	for (int i = 0; i < s->h; i++)
	{
		for (int j = 0; j < s->w; j++)
			putchar(s->map[i][j]);
		putchar('\n');
	}
	free_map(s);
}
void init_map(t* s, char* av[])
{
	s->w = atoi(av[1]);
	s->h = atoi(av[2]);
	s->i = atoi(av[3]);
	s->pp[0] = 0;
	s->pp[1] = 0;
	s->xp[0] = -1;
	s->xp[1] = -1;
	s->on = false;
	s->map = make_map(s);
}

int neigbor(t* s, int i, int j)
{
	int count = 0;
	
	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			if ((!(!x && !y )) && in_map(s, (j + x), (i + y)) && s->map[i + y][j + x] == ALIVE)
				count++;
		}
	}
	return (count);
	
}

void life(t* s)
{
	while (s->i--)
	{
		char** new_map = make_map(s);
		for (int i = 0; i < s->h; i++)
		{
			for (int j = 0; j < s->w; j++)
			{
				int n = neigbor(s, i, j);
				if (s->map[i][j] == ALIVE)
				{
					if (n < 2)							//underpopulation
							new_map[i][j] = DEAD;
					else if (n <= 3)
							new_map[i][j] = ALIVE;
					else if (n > 3)						//overpopulation
							new_map[i][j] = DEAD;				
				}					 
				else if (s->map[i][j] == DEAD && n == 3) //reproduction
					new_map[i][j] = ALIVE;
			}
		}
		free_map(s);
		s->map = new_map;
	}
}
int main (int ac, char** av)
{
	int rb;
	char buff;
	t s;

	if (ac != 4)
		return (1);
	init_map(&s, av);
	
	do
	{
		rb = read(0, &buff, 1);
		get_pos(buff, &s);
	}
	while (rb);

	if (s.i)
		life(&s);
	pr_map(&s);
	return (0);
}
