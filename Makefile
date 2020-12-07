build:
	g++ -o chess main.cpp boardFunctions.cpp xboardFunctions.cpp minimax.cpp utility.cpp

run:
	./chess

run_etapa1:
	xboard -fcp "./chess" -debug

run_etapa2:
	xboard -fcp "./chess" -scp "fairymax" -secondInitString "new\nrandom\nsd 2\n" -tc 5 -inc 2 -autoCallFlag true -mg 10 -sgf partide.txt -reuseFirst false -debug

run_etapa33:
	xboard -fcp "./chess" -scp "fairymax" -secondInitString "new\nrandom\nsd 3\n" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false -debug

run_etapa34:
	xboard -fcp "./chess" -scp "fairymax" -secondInitString "new\nrandom\nsd 4\n" -tc 5 -inc 2 -autoCallFlag true -mg 4 -sgf partide.txt -reuseFirst false -debug

clean:
	rm chess xboard.debug partide.txt
	