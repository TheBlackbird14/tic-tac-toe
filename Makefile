cgl: main.c games/tictactoe.c libs/strcap.c libs/cs50.c
	cc -o games/tictactoe games/tictactoe.c libs/strcap.c libs/cs50.c
	cc -w -o cgl main.c libs/strcap.c libs/cs50.c

bships: games/battleships.c libs/strcap.c libs/cs50.c libs/colors.h
	cc -o games/battleships games/battleships.c libs/strcap.c libs/cs50.c