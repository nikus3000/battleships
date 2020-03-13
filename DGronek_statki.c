#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<time.h>
#define size 10
#include <unistd.h>

char player[size][size];
char computer[size][size];
char computer_sunk[size][size]; 
char player_sunk[size][size]; 
int length[4] = {4,3,2,1};
/* number of player's ship which were sunk */
int killed_player = 0;
/* number of computer's ship which were sunk */
int killed_computer = 0;
/* how long is the current ship being sunk by the computer*/
int cur_len = 0;
/* 0 - ship of given length hasn't been sunk yet; 1 - it has been sunk */
int sunk_ship[4] = {0,0,0,0};
/* previous move coordinates */
int x_old, y_old, x_older, y_older, x_olderer, y_olderer; 
int xc, yc;

int max (int a, int b) { return a > b ? a : b; }

int min (int a, int b) { return a > b ? b : a; }

/* Funkcja przygotowujaca tablice na statki */
void prepare_tables()
{
    int i;
    for(i=0;i<size;i++)
    {
	int j;
	for(j=0; j<size; j++)
	{
	    player[i][j]='.';
	    computer[i][j]='.';
	    player_sunk[i][j]='.';
	    computer_sunk[i][j]='.';
	}
    }
}

/* Funkcja pobierajaca od gracza wspolrzedne statkow */
void place_players_ships()
{
    int i;
    printf("======GRA W STATKI======\n");
    sleep(1);
    printf("Plansza ma rozmiary 10 x 10.\n");
    sleep(1);
    printf("Wiersze i kolumny maja numery od 0 do 9\n\n");
    sleep(2);
    printf("Umiesc statki na planszy\n");
    for(i=0; i<4; i++)
    {
	int len = length[i];
	int p;
	int correct;
	int x,y;
	int i,j;
	while(1)
	{
	    printf("Jesli statek ma byc ulozony poziomo, wcisnij 0, jesli pionowo - 1\n");
	    scanf("%d",&p);
	    if(p==0 || p==1)
		break;
	    else
		printf("Źle podany kierunek statku\n");
	}

	while(1)
	{
	    printf("Podaj wsp x i y poczatku statku o długości %d\n",len);
	    scanf("%d %d",&x,&y);
	    correct = 1;
	    if(p == 0)
	    {
		int i;
		if(y+len>size) correct = 0;
		if(x>size-1) correct = 0;
		/* loop over rows */
		for( i = -1; i<2; i++)
		{
		    int row = x + i;
		    if(row>=0 && row < size)
		    {
			/* loop over row elelements */
			int j;
			for(j=-1; j<=len; j++)
			{
			    int column = y + j;
			    if(column>=0 && column<size && player[row][column] == 'x') 
				correct = 0;
			}
		    }
		}
	    }
	    if(p == 1)
	    {
		int i;
		if(x+len>size) correct = 0;
		if(y>size-1) correct = 0;
		/* loop over columns */
		for(i = -1; i<2; i++)
		{
		    int column = y + i;
		    if(column>=0 && column< size)
		    {
			/* loop over column elelements */
			int j;
			for(j=-1; j<=len; j++)
			{
			    int row = x + j;
			    if(row>=0 && row<size && player[row][column] == 'x') 
				correct = 0;
			}
		    }
		}
	    }
	    if(correct == 1) break;
	    else
		printf("Źle podane współrzędne statku\n");
	}
	for(i=0; i<len; i++)
	{
	    /* if horizontal */
	    if(p == 0)
		player[x][y+i] = 'x';
	    /* if vertical */
	    if(p == 1)
		player[x+i][y] = 'x';
	}

	printf("  ");
	for(j=0; j<size; j++)
	    printf("%d ",j);
	printf("\n");
	for(i=0; i<size; i++)
	{
	    int j;
	    printf("%d ",i);
	    for(j=0; j<size; j++)
		printf("%c ",player[i][j]);
	    printf("\n");
	    
	}
    }
}

/* Funkcja rozmieszczajaca statki komputera */
void place_computers_ships()
{
    /* for computer */
    int i;
    srand(time(NULL));
    for(i=0; i<4; i++)
    {
	int len = length[i];
	int pc = rand()%2;
	int range = size - len + 1;
	int xc, yc;
	int correct;
	int i;
	while(1)
	{
	    correct = 1;
	    if(pc == 0)
	    {
		int i;
		xc = rand()%size;
		yc = rand()%range;
		/* loop over rows */
		for(i = -1; i<2; i++)
		{
		    int row = xc + i;
		    if(row>=0 && row < size)
		    {
			/* loop over row elelements */
			int j;
			for(j=-1; j<=len; j++)
			{
			    int column = yc + j;
			    if(column>=0 && column<size && computer[row][column] == 'x') 
				correct = 0;
			}
		    }
		}
	    }
	    if(pc == 1)
	    {
		int i;
		xc = rand()%range;
		yc = rand()%size;
		/* loop over columns */ 
		for(i = -1; i<2; i++)
		{
		    int column = yc + i;
		    if(column>=0 && column< size)
		    {
			/* loop over column elelements */
			int j;
			for(j=-1; j<=len; j++)
			{
			    int row = xc + j;
			    if(row>=0 && row<size && computer[row][column] == 'x') 
				correct = 0;
			}
		    }
		}
	    }
	    if(correct) break;
	}
	for(i=0; i<len; i++)
	{
	    /* if horizontal */
	    if(pc == 0)
		computer[xc][yc+i] = 'x';
	    if(pc == 1)
		computer[xc+i][yc] = 'x';
	}
    }

    /*
     * wyświetlanie statkow rozmieszczonych przez komputer
    printf("  ");
    for(int j=0; j<size; j++)
	printf("%d ",j);
    printf("\n");
    for(int i=0; i<size; i++)
    {
	printf("%d ",i);
	for(int j=0; j<size; j++)
	    printf("%c ",computer[i][j]);
	printf("\n");
	
    }
	*/
}

/* Funkcja realizujaca ruch gracza */
void players_move()
{
    int x,y;
    int j,i;
    printf("\n======Ruch gracza=======\n");
    while(1)
    {
	printf("Podaj współrzędne x i y oddzielone spacją, w które chcesz strzelić\n");
	scanf("%d %d",&x,&y);
	if(x>=0 && x<size && y>=0 && y<size && computer_sunk[x][y] == '.')
	    break;
	else
	    printf("Współrzędne podane niepoprawnie.");
    }
    if(computer[x][y]=='.')
    {
	computer_sunk[x][y]='*';
	printf("Pudło!\n");
    }
    if(computer[x][y]=='x')
    {
	computer_sunk[x][y]='o';
	printf("Trafiony!\n");
	killed_computer++;
    }
    printf("  ");
    for(j=0; j<size; j++)
	printf("%d ",j);
    printf("\n");
    for(i=0; i<size; i++)
    {
	int j;
	printf("%d ",i);
	for(j=0; j<size; j++)
	    printf("%c ",computer_sunk[i][j]);
	printf("\n");
	
    }
}

/* STRATEGIA RUCHOW KOMPUTERA */

/* Wybor ostrzeliwanych wspolrzednych, */
/* jesli komputer nie zaczal jeszcze ostrzalu nowego statku */
void cur_len0()
{
    xc = rand()%size;
    yc = rand()%size;
}

/* Wybor ostrzeliwanych wspolrzednych, */
/* jesli komputer zestrzelil juz pojedynczy segment nowego statku */
void cur_len1()
{
    int shot = 0;
    int exist = 0;
    if((y_old-1)>=0)
    {
	exist++;
	if(player_sunk[x_old][y_old-1]!='.') shot++;
    }
    if((y_old+1)<size)
    {
	exist++;
	if(player_sunk[x_old][y_old+1]!='.') shot++;
    }
    if((x_old-1)>=0)
    {
	exist++;
	if(player_sunk[x_old-1][y_old]!='.') shot++;
    }
    if((x_old+1)<size)
    {
	exist++;
	if(player_sunk[x_old+1][y_old]!='.') shot++;
    }
    if(shot<exist && (sunk_ship[0]==0 || sunk_ship[1]==0 || sunk_ship[2]==0))
    {
	/* horizonal/vertical */
	int pp = rand()%2; 
	/* n1 - vertical shift; n2 - horizontal shift */
	int n1, n2;
	if(pp == 0)
	{
	    n1 = 0;
	    n2 = -1 + rand()%3;
	}
	else
	{
	    n1 = -1 + rand()%3;
	    n2 = 0;
	}
	xc = x_old + n1;
	yc = y_old + n2;
    }
    else
    {
	/* ship of length 1 was found */
	sunk_ship[3] = 1;
	cur_len = 0;
	/* incorrect coordinates so that we come back to the begining of the loop */
	xc = -1;
	yc = -1;
    }
}

/* Wybor ostrzeliwanych wspolrzednych, */
/* jesli komputer zestrzelil juz dwa segmenty nowego statku */
void cur_len2()
{
    if(sunk_ship[0]==0 || sunk_ship[1]==0)
    {
	if(x_old == x_older)
	{
	    int y1 = min(y_old,y_older)-1;
	    int y2 = max(y_old,y_older)+1;
	    xc = x_old;
	    if((y1<0 || player_sunk[xc][y1]!='.') && (y2 >= size || player_sunk[xc][y2]!='.'))
	    {
		/* ship of length 2 was found */
		sunk_ship[2] = 1;
		cur_len = 0;
		/* incorrect coordinates so that we come back to the begining of the loop */
		xc = -1;
		yc = -1;
	    }
	    else
	    {
		int n = rand()%2;
		if(n == 0)
		    yc = y1;
		else
		    yc = y2;
	    }
	}
	if(y_old == y_older)
	{
	    int x1 = min(x_old,x_older)-1;
	    int x2 = max(x_old,x_older)+1;
	    yc = y_old;
	    if((x1<0 || player_sunk[x1][yc]!='.') && (x2>=size || player_sunk[x2][yc]!='.'))
	    {
		/* ship of length 2 was found */
		sunk_ship[2] = 1;
		cur_len = 0;
		/* incorrect coordinates so that we come back to the begining of the loop */
		xc = -1;
		yc = -1;
	    }
	    else
	    {
		int n = rand()%2;
		if(n == 0)
		    xc = x1;
		else
		    xc = x2; 
	    }
	}
    }
    else
    {
	/* ship of length 2  was found */
	sunk_ship[2] = 1;
	cur_len = 0;
	xc = -1;
	yc = -1;
    }
}

/* Wybor ostrzeliwanych wspolrzednych, */
/* jesli komputer zestrzelil juz trzy segmenty nowego statku */
void cur_len3()
{
    if(sunk_ship[0]==0)
    {
	if(x_old == x_older)
	{
	    int y1 = min(y_old,min(y_older,y_olderer))-1;
	    int y2 = max(y_old,max(y_older,y_olderer))+1;
	    xc = x_old;
	    if((y1<0 || player_sunk[xc][y1]!='.') && (y2>=size || player_sunk[xc][y2]!='.'))
	    {
		/* ship of length 3  was found */
		sunk_ship[1] = 1;
		cur_len = 0;
		/* incorrect coordinates so that we come back to the begining of the loop */
		xc = -1;
		yc = -1;
	    }
	    else
	    {
		int n = rand()%2;
		if(n == 0)
		    yc = y1; 
		else
		    yc = y2; 
	    }
	}
	if(y_old == y_older)
	{
	    int x1 = min(x_old,min(x_older,x_olderer))-1;
	    int x2 = max(x_old,max(x_older,x_olderer))+1;
	    yc = y_old;
	    if((x1<0 || player_sunk[x1][yc]!='.') && (x2>=size || player_sunk[x2][yc]!='.'))
	    {
		/* ship of length 3 was found */
		sunk_ship[1] = 1;
		cur_len = 0;
		/* incorrect coordinates so that we come back to the begining of the loop */
		xc = -1;
		yc = -1;
	    }
	    else
	    {
		int n = rand()%2;
		if(n == 0)
		    xc = x1; 
		else
		    xc = x2; 
	    }
	}
    }
    else
    {
	/* ship of length 3  was found */
	sunk_ship[1] = 1;
	cur_len = 0;
	xc = -1;
	yc = -1;
    }
}

/* Wybor wspolrzednych ostrzeliwanych przez komputer */
void choose_shooting_point()
{
    printf("======Ruch komputera=======\n");
    while(1)
    {
	if(cur_len == 0) cur_len0();
	if(cur_len == 1) cur_len1();
	if(cur_len == 2) cur_len2();
	if(cur_len == 3) cur_len3();
	if(xc>=0 && xc<size && yc>=0 && yc<size && player_sunk[xc][yc] == '.')
	    break;
    }
}

/* Ruch komputera */
void computers_move()
{
    int i,j;
    choose_shooting_point();
    if(player[xc][yc]=='.')
    {
	player_sunk[xc][yc]='*';
	printf("Pudło!\n");
    }
    if(player[xc][yc]=='x')
    {
	player_sunk[xc][yc]='o';
	if(cur_len==0)
	{
	    x_old = xc;
	    y_old = yc;
	}
	if(cur_len==1)
	{
	    x_older = x_old;
	    y_older = y_old;
	    x_old = xc;
	    y_old = yc;
	}
	if(cur_len==2)
	{
	    x_olderer = x_older;
	    y_olderer = y_older;
	    x_older = x_old;
	    y_older = y_old;
	    x_old = xc;
	    y_old = yc;
	}
	printf("Trafiony!\n");
	cur_len++;
	/* ship of length 4  was found */
	if (cur_len == 4) sunk_ship[0]=1;
	cur_len = cur_len%4;
	killed_player++;
    }
    printf("  ");
    for(j=0; j<size; j++)
	printf("%d ",j);
    printf("\n");
    for(i=0; i<size; i++)
    {
	int j;
	printf("%d ",i);
	for(j=0; j<size; j++)
	    printf("%c ",player_sunk[i][j]);
	printf("\n");
	
    }
}

/* Funkcja nawigujaca rozgrywka */
void play()
{
    /* PLAY! */
    while(killed_player<10 && killed_computer<10)
    {
	/* player's move */
	players_move();
	if(killed_computer == 10)
	{
	    printf("Wygrałeś!\n");
	    break;
	}
	printf("\n");
	printf("Teraz ruch komputera...\n\n");
	sleep(2);
	
	/*computer's move */
	computers_move();
	if(killed_player == 10)
	{
	    printf("Przegrałeś!\n");
	    break;
	}
    }
}

int main()
{
    prepare_tables();
    place_players_ships();
    place_computers_ships();
    play();

}
