// GameOfLife_withMPI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdc++.h"
#include "mpi.h"

using namespace std;

///
/// @brief main function
/// 
/// @param argc number of argumnet passed to the program; should be 4
/// @param argv array of char pointer argument passed to the program;
///
/// @return returns 0 if sucessusful
/// 
int main(int argc, char* argv[])
{
    // initialization of mpi
	MPI_Init(&argc, &argv);

	// getting the total number of process
	int mpiSize;
	MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	// assigning uniqe rank to each process
	int mpiRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);

	// root
	int mpiRoot = 0;

	int nRows = 0; // rows of grid
	int nCols = 0; // column of grid
	int nTime = 0; // timestep

	// root process gets the variables from the command line argument
	if (mpiRank == mpiRoot)
	{
		// error checking
		if (argc != 4)
		{
			cerr << "incorrected number of command-line arguments, 4 expected!!!" << endl;
			cerr << "usage: <binaryFile> <number of Rows> <number of Columns> <Time Step>" << endl;
		}

		// initializing the values
		nRows = atoi(argv[1]);
		nCols = atoi(argv[2]);
		nTime = atoi(argv[3]);

		// errorChecking
		if (nRows < 1 || nCols < 1 || nTime < 1)
		{
			cerr << "invalid command-line arguments" << endl;
		}
	}

	// boardcasting the variables
	MPI_Bcast(&nRows, 1, MPI_INT, mpiRoot, MPI_COMM_WORLD);
	MPI_Bcast(&nCols, 1, MPI_INT, mpiRoot, MPI_COMM_WORLD);
	MPI_Bcast(&nTime, 1, MPI_INT, mpiRoot, MPI_COMM_WORLD);

	// partitioning the grid
	int nRowsLocal = nRows / mpiSize;

	// as the above division is int division, they may be some remainders
	// these rows will be processed by last process
	if (mpiRank == mpiSize - 1)
	{
		nRowsLocal += nRows % mpiSize;
	}

	int nRowsLocalWithGhost = nRowsLocal + 2;
	int nColsLocalWithGhost = nCols + 2;


	// we need two grid because we may need curr grid info and do not want 
	// to change that when updating the current grid to get next grid

	// matrix to store the current grid
	vector< vector<int> > currGrid(nRowsLocalWithGhost);

	// setting up the current grid
	for (int i = 0; i < nColsLocalWithGhost; ++i)
	{
		currGrid.at(0) = vector<int>(nColsLocalWithGhost);
	}
	
	// matrix to store the next grid
	vector< vector<int> > nextGrid(nRowsLocalWithGhost);

	// setting up the current grid
	for (int i = 0; i < nColsLocalWithGhost; ++i)
	{
		nextGrid.at(0) = vector<int>(nColsLocalWithGhost);
	}

	// randamly innializing the grid with 0 or 1; where 0 is dead and 1 is alive
	for (int iRow = 1; iRow <= nRows ; ++iRow)
	{
		for (int iCol = 1; iCol <= nCols; ++iCol)
		{
			currGrid[iRow][iCol] = rand() % 2;
		}
	}

	// getting the rank where upper and lower Neighbour are
	int upperNeighborRank = (mpiRank == 0) ? mpiSize - 1 : mpiRank - 1;
	int lowerNeighborRank = (mpiRank == mpiSize -1) ? 0 : mpiRank + 1;

	const int ALIVE = 1;	// alive cell value
	const int DEAD = 0;		// dead cell value

	// time loop
	for (int iTime = 0; iTime < nTime; ++iTime)
	{
		// ======== setting up the currGrid ========
		// sending info to neighbour
		// send top row info to above
		MPI_Send(&currGrid[1][0], nColsLocalWithGhost, MPI_INT, upperNeighborRank, 0, MPI_COMM_WORLD);
		// send bottom row info to below
		MPI_Send(&currGrid[nRowsLocal][0], nColsLocalWithGhost, MPI_INT, lowerNeighborRank, 0, MPI_COMM_WORLD);

		// recieving info from neighbour
		// recieve top row from above
		MPI_Recv(&currGrid[0][0], nColsLocalWithGhost, MPI_INT, upperNeighborRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// recieve bottom row info from below
		MPI_Recv(&currGrid[nRowsLocal + 1][0], nColsLocalWithGhost, MPI_INT, lowerNeighborRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// ghost columns 
		for (int iRow = 0; iRow < nRowsLocalWithGhost; ++iRow)
		{
			currGrid[iRow][0] = currGrid[iRow][nCols]; // first col = ghost grid == second last col = last cell of actual grid
			currGrid[iRow][nCols + 1] = currGrid[iRow][1] ; // last col = ghost grid == second col = first cell of actual grid
		}

		// display current grid on screen

		// ======== updating the grid ========

		for (int iRow = 1; iRow <= nRowsLocal; ++iRow)
		{
			for (int iCol = 1; iRow <= nCols; ++iCol)
			{
				int nAliveNeighbors = 0; // keeps track of alive neighbors

			}
		}



	}


	// cleaning
	MPI_Finalize();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
