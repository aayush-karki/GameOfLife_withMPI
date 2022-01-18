#Game of Life

Game of life operates on time step basis

Game of life follows 4 rule:
	-If a cell has fewer than 2 ALIVE neighbors, it will be DEAD in the next step.
	-If an ALIVE cell has 2 or 3 ALIVE neighbors, it will be ALIVE in the next step.
	-If ancell has more than 3 ALIVE neighbors, it will be DEAD in the next step.
	-If a DEAD cell has 3 ALIVE neighbors, it will be ALIVE in the next step.

Moore neighbour is used rather than Von Neumann neighbour. 
	So, there are 8 neighbour for any given cell

Edges cells inplement Ghost row as neighbour.

	Given the following cell grids configuration:
	-------------------------
	| 1 | 0 | 0 | 0 | 1 | 0 |
	| 0 | 1 | 0 | 0 | 0 | 1 |
	| 0 | 0 | 0 | 1 | 0 | 0 |
	| 0 | 1 | 0 | 0 | 0 | 0 |
	| 0 | 0 | 0 | 0 | 1 | 0 |
	| 1 | 0 | 0 | 1 | 1 | 0 |
	-------------------------
	The overall gird with the Ghost row will look like:

			| 1 | 0 | 0 | 1 | 1 | 0 |				<---------- Ghost Row == sixth row

			-------------------------
	| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Row == 1
	| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |		<---------- Row == 2
	| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |		<---------- Row == 3
	| 0 |	| 0 | 1 | 0 | 0 | 0 | 0 |  | 0 |		<---------- Row == 4
	| 0 |	| 0 | 0 | 0 | 0 | 1 | 0 |  | 0 |		<---------- Row == 5
	| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |		<---------- Row == 6
			-------------------------

			| 1 | 0 | 0 | 0 | 1 | 0 |				<---------- Ghost Row == first row

	  ^									 ^
	  |									 |
	  |									 |	
	Ghost Row == Sixth Coulmn			Ghost Row == first Coulmn


	only the diagnal neighbours of corner piece are missing. 
	so we take the opposite diagnal corner for that

	| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |			<---------- opposite diagnal corner cell 

			-------------------------
	| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |			<---------- Row == 1
	| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |			<---------- Row == 2
	| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |			<---------- Row == 3
	| 0 |	| 0 | 1 | 0 | 0 | 0 | 0 |  | 0 |			<---------- Row == 4
	| 0 |	| 0 | 0 | 0 | 0 | 1 | 0 |  | 0 |			<---------- Row == 5
	| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |			<---------- Row == 6
			-------------------------

	| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |			<---------- opposite diagnal corner cell 


The grids are partitioned by using the following formula numberOfRows / numberOfTotalProcess,
	where the last process also process the remainded rows if any.

	In above grid if we have 4 process, the grid is partitioned like this 

	

	process 1:
				| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |		<---------- Ghost Row == sixth row
				| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Row == 1
	process 2:
				| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |		<---------- Row == 2
	process 3:
				| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |		<---------- Row == 3
	process 4:
				| 0 |	| 0 | 1 | 0 | 0 | 0 | 0 |  | 0 |		<---------- Row == 4
				| 0 |	| 0 | 0 | 0 | 0 | 1 | 0 |  | 0 |		<---------- Row == 5
				| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |		<---------- Row == 6
				| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Ghost Row == first row
	

So, as each process will have a portion of the grid, the top and bottom grid of each 
	process will have to get access to the neighbouring grid from other process.
	This is done using the mpi_send and mpi_recive funtion.
	
So, our final grid after partitioning will look like this:

	process 1:
				| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |		<---------- Ghost Row == sixth row
				| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Row == 1
				| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |		<---------- Ghost Row == second row
	process 2:
				| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Ghost Row == first row
				| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |		<---------- Row == 2
				| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |		<---------- Ghost Row == third row
	process 3:
				| 1 |	| 0 | 1 | 0 | 0 | 0 | 1 |  | 0 |		<---------- Ghost Row == second row
				| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |		<---------- Row == 3
				| 0 |	| 0 | 1 | 0 | 0 | 0 | 0 |  | 0 |		<---------- Ghost Row == fourth row
	process 4:
				| 0 |	| 0 | 0 | 0 | 1 | 0 | 0 |  | 0 |		<---------- Ghost Row == third row
				| 0 |	| 0 | 1 | 0 | 0 | 0 | 0 |  | 0 |		<---------- Row == 4
				| 0 |	| 0 | 0 | 0 | 0 | 1 | 0 |  | 0 |		<---------- Row == 5
				| 0 |	| 1 | 0 | 0 | 1 | 1 | 0 |  | 1 |		<---------- Row == 6
				| 0 |	| 1 | 0 | 0 | 0 | 1 | 0 |  | 1 |		<---------- Ghost Row == first row

	Here, each process will send their top and bottom row to the process who have correspoing ghost row