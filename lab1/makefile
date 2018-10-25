I = float-observation
J = error
K = less_error

zad1: Mownit-Lab1.ipynb
		jupyter-notebook

zad3: $(I).c
		gcc -Wall -I/home/crunchy/bin/gsl/include -c $(I).c
		gcc -L/home/crunchy/bin/gsl/lib $(I).o -lgsl -lgslcblas -lm -o $(I)
		./$(I)

zad4: $(J).c $(K).c
		gcc $(J).c -o $(J)
		gcc $(K).c -o $(K)
		./$(J)
		./$(K)
		