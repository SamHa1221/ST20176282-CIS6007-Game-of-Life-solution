//
// Array Visualisation for CIS6007 WRIT1
//

#include <iostream>
#include <string>
#include <random>
#include "gl_stuff.h"
#include "Array2D.h"
#include <tbb/tbb.h>
#include <chrono>


using namespace std;
using namespace tbb;


#pragma region Function Prototypes

void initPattern(Array2D<int>& array); // Initialise grid with "alive" and "dead" cell patterns
void update(void); // Main update function - apply Game of Life rules here

#pragma endregion


//
// Example array
//
Array2D<int>						myArray = Array2D<int>(64, 64);
Array2D<int> tempArray = Array2D<int>(myArray.w, myArray.h);


// Starting point for our program...
int main(int argc, char* argv[])
{
	// Initialise OpenGL window...
	gl_init(argc, argv, update, "CIS6007 WRIT1 - Game of Life Demo", true);

	// Initialise our Game of Life starting pattern...
	initPattern(myArray);

	// Start the main loop - this calls our update function (below) to update our game of life generations.
	// Note: We never return from glutMainLoop until we quit the application
	glutMainLoop();

    return 0;
}


// Initialise grid with "alive" and "dead" cell patterns
void initPattern(Array2D<int>& array) {

	// 1. First clear the array (set all cells to "dead" ie. 0)
	parallel_for(
		0,
		array.w * array.h,
		[&](int i) {

			int x = i % array.w;
			int y = i / array.h;
			array(x, y) = 0;
		}
	);

	// Setup some cells to be "alive" - this is the initial state
	// NOTE: *** REPLACE THIS WITH YOUR OWN CODE TO SETUP THE LIVE CELL PATTERNS
	/*parallel_for(
		0,
		array.h,
		[&](int y) {
			for (int x = 0; x < array.w; x++) {
				if (x == y) {
					array(x, y) = 1;
				}
			}
		}
	);*/
	array(0, 0) = 1;
	array(1, 0) = 1;
	array(0, 1) = 1;
	array(1, 1) = 1;
}

// Main update function - apply Game of Life rules here
// Note: This is called by FreeGLUT's main event loop - YOU DON'T NEED TO CALL THIS - IT'S CALLED FOR YOU!!!
void update(void) {

	/*//static int generation = 0; // keeps track of the current generation

	// Define the number of generations to run the game of life
	//int numberOfGenerations = 25;

	// Loop until the desired number of generations is reached
	//if (generation < numberOfGenerations) {

		// Clear the tempArray (set all cells to "dead" ie. 0)
		initPattern(tempArray);

		// Start measuring the time taken
		auto start_time = chrono::high_resolution_clock::now();

		// Loop through every cell in the array
		for (int y = 1; y < myArray.h - 1; y++) {
			for (int x = 1; x < myArray.w - 1; x++) {

				// Get the number of live neighbors for this cell
				int liveNeighbors = 0;

				// Loop through the grid on this cell
				for (int j = y - 1; j <= y + 1; j++) {
					for (int i = x - 1; i <= x + 1; i++) {

						// Skip the current cell
						if (i == x && j == y) {
							continue;
						}

						// Check if the neighboring cell is alive
						if (myArray(i, j) == 1) {
							liveNeighbors++;
						}
					}
				}

				// Apply the Game of Life rules
				if (myArray(x, y) == 1 && (liveNeighbors < 2 || liveNeighbors > 3)) {
					tempArray(x, y) = 0; // current live cell dies
				}
				else if (myArray(x, y) == 0 && liveNeighbors == 3) {
					tempArray(x, y) = 1; // dead cell comes to life
				}
				else {
					tempArray(x, y) = myArray(x, y); // cell stays the same
				}
			}
		}

		// Copy the temporary array to the main array
		myArray = tempArray;


		// Stop measuring the time taken and calculate the duration
		auto end_time = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

		// Output the duration to the console
		cout << "Time taken for this update: " << duration.count() << " microseconds." << endl;

		// Redraw the screen to show latest generation
		renderLatestGeneration(myArray);

		// Increment the generation counter
		//generation++;
	}
//}*/

   static int generation = 0; // keeps track of the current generation

	// Define the number of generations to run the game of life
	int numberOfGenerations = 25;

	// Loop until the desired number of generations is reached
	if (generation < numberOfGenerations) {


		// Clear the tempArray (set all cells to "dead" ie. 0)
		initPattern(tempArray);

		// Start measuring the time taken
		auto start_time = chrono::high_resolution_clock::now();

		// Loop through every cell in the array in parallel
		parallel_for(0, myArray.h, [&](int y) {
			for (int x = 0; x < myArray.w; x++) {

				// Get the number of live neighbors for this cell
				int liveNeighbors = 0;

				// Loop through the grid on this cell
				for (int j = y - 1; j <= y + 1; j++) {
					for (int i = x - 1; i <= x + 1; i++) {

						// Skip the current cell
						if (i == x && j == y) {
							continue;
						}

						// Check if the neighboring cell is alive
						if (myArray(i, j) == 1) {
							liveNeighbors++;
						}
					}
				}

				// Apply the Game of Life rules
				if (myArray(x, y) == 1 && (liveNeighbors < 2 || liveNeighbors > 3)) {
					tempArray(x, y) = 0; // current live cell dies
				}
				else if (myArray(x, y) == 0 && liveNeighbors == 3) {
					tempArray(x, y) = 1; // dead cell comes to life
				}
				else {
					tempArray(x, y) = myArray(x, y); // cell stays the same
				}
			}
			});

		// Copy the temporary array to the main array
		myArray = tempArray;


		// Stop measuring the time taken and calculate the duration
		auto end_time = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

		// Output the duration to the console
		cout << "Time taken for this update: " << duration.count() << " microseconds." << endl;

		// Redraw the screen to show latest generation
		renderLatestGeneration(myArray);

		// Increment the generation counter
		generation++;
	}
}