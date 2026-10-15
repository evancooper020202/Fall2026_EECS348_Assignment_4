/*
 * ==============================================================================
 * Program Name: EECS 348 Assignment 4 - Object-Oriented Sudoku Solver
 * Author: $Evan Cooper
 * Creation Date: October 10, 2026
 * 
 * Description:
 * An object-oriented C++ program that solves 9x9 Sudoku puzzles using a 
 * depth-first search (DFS) algorithm with backtracking. It encapsulates board 
 * logic and solving routines into classes, processes input puzzle files where 
 * blank cells are represented by underscores ('_'), and prints the file name, 
 * initial puzzle configuration, and all valid solutions.
 * 
 * Inputs:
 * - Text files containing 9x9 Sudoku puzzles (e.g., puzzle1.txt, puzzle2.txt, 
 *   puzzle3.txt, puzzle4.txt, puzzle5.txt)
 * 
 * Output:
 * - Puzzle file name, initial puzzle grid printout, and all discovered solutions 
 *   (or "No solution found" if no valid grid configuration exists).
 * 
 * Collaborators:
 * - None (or list the names of any classmates you worked with)
 * 
 * Other Sources for the Code:
 * - ChatGPT and Gemini (GenAI tools utilized for code generation structure and 
 *   analysis as permitted by the assignment instructions)
 * ==============================================================================
 */
#include <iostream> // Include standard input-output stream library for console logging
#include <fstream>  // Include file stream library for parsing puzzle files
#include <vector>   // Include vector container library for dynamic arrays
#include <string>   // Include string library for handling file names

using namespace std; // Use standard namespace to simplify code syntax

class SudokuPuzzle { // Define class SudokuPuzzle to encapsulate board state and solver logic
private: // Access specifier for private internal member variables and methods
    static const int GRID_SIZE = 9; // Declare constant grid dimension of 9 for standard Sudoku
    int board[GRID_SIZE][GRID_SIZE]; // 2D integer array representing the working grid state
    int initial_board[GRID_SIZE][GRID_SIZE]; // 2D integer array storing the original puzzle configuration
    int solution_count; // Counter tracking the number of valid solutions discovered

    bool isSafe(int row, int col, int num) { // Helper method to check if placement of num at (row, col) is valid
        for (int i = 0; i < GRID_SIZE; i++) { // Loop through rows and columns to check existing digits
            if (board[row][i] == num || board[i][col] == num) { // Check if digit already exists in row or column
                return false; // Return false if conflict is detected
            } // End of row/column conflict check if block
        } // End of row/column traversal loop

        int startRow = row - row % 3; // Calculate the top-left row index of the 3x3 subgrid
        int startCol = col - col % 3; // Calculate the top-left column index of the 3x3 subgrid
        for (int r = 0; r < 3; r++) { // Loop through the 3 rows of the 3x3 subgrid
            for (int c = 0; c < 3; c++) { // Loop through the 3 columns of the 3x3 subgrid
                if (board[startRow + r][startCol + c] == num) { // Check if digit exists inside the 3x3 block
                    return false; // Return false if conflict is detected in the subgrid
                } // End of subgrid cell check if block
            } // End of subgrid column loop
        } // End of subgrid row loop

        return true; // Return true if no conflicts found in row, column, or subgrid
    } // End of isSafe method

    void solve(int row, int col) { // Recursive method performing DFS with backtracking
        if (row == GRID_SIZE - 1 && col == GRID_SIZE) { // Base case: reached past the last grid cell
            solution_count++; // Increment the solution counter
            cout << "\nSolution #" << solution_count << ":" << endl; // Print solution header banner
            printBoard(board); // Output the solved board grid configuration
            return; // Return from current recursive frame
        } // End of base case check if block

        if (col == GRID_SIZE) { // Check if current row has been fully traversed
            row++; // Increment row index to move down to the next row
            col = 0; // Reset column index back to 0
        } // End of row wrap-around if block

        if (board[row][col] != 0) { // Check if current cell contains a pre-filled initial clue
            solve(row, col + 1); // Recursively advance to the next cell without modifying current
            return; // Return after recursive branch finishes
        } // End of pre-filled cell skip if block

        for (int num = 1; num <= GRID_SIZE; num++) { // Loop through candidate digits 1 through 9
            if (isSafe(row, col, num)) { // Check if candidate digit is safe to place
                board[row][col] = num; // Place candidate digit into cell (Choose)
                solve(row, col + 1); // Recursively solve for the next cell (Explore)
                board[row][col] = 0; // Reset cell back to 0 upon return (Backtrack)
            } // End of safety check if block
        } // End of candidate digit loop
    } // End of solve method

    void printBoard(const int b[GRID_SIZE][GRID_SIZE]) const { // Helper method to output any grid state
        for (int r = 0; r < GRID_SIZE; r++) { // Loop through each row of the grid
            for (int c = 0; c < GRID_SIZE; c++) { // Loop through each column in the current row
                if (b[r][c] == 0) { // Check if cell is empty (represented by 0 internally)
                    cout << "_ "; // Print underscore for empty cells per assignment formatting rules
                } else { // Otherwise, cell contains a valid digit
                    cout << b[r][c] << " "; // Print the digit followed by a space separator
                } // End of cell formatting if-else block
            } // End of column print loop
            cout << endl; // Output newline character at the end of each row
        } // End of row print loop
    } // End of printBoard method

public: // Access specifier for public constructors and interface methods
    SudokuPuzzle() : solution_count(0) { // Constructor initializing solution counter to zero
        for (int i = 0; i < GRID_SIZE; i++) { // Loop through rows to initialize board arrays
            for (int j = 0; j < GRID_SIZE; j++) { // Loop through columns to initialize board arrays
                board[i][j] = 0; // Initialize working board cell to 0
                initial_board[i][j] = 0; // Initialize initial board cell to 0
            } // End of inner initialization loop
        } // End of outer initialization loop
    } // End of constructor

    bool loadPuzzle(const string& filename) { // Method to load puzzle configuration from a text file
        ifstream file(filename); // Open file stream with specified filename
        if (!file.is_open()) { // Check if file stream successfully opened
            cerr << "Error opening file: " << filename << endl; // Output error message to standard error
            return false; // Return false indicating failure to open file
        } // End of file open check if block

        char ch; // Character variable to hold incoming file stream characters
        int r = 0, c = 0; // Row and column index trackers for loading the grid
        while (file >> ch && r < GRID_SIZE) { // Read file characters sequentially up to grid limits
            if (ch == '_') { // Check if character represents an empty cell underscore
                board[r][c] = 0; // Set working grid cell to 0
                initial_board[r][c] = 0; // Set initial board cell to 0
            } else if (ch >= '1' && ch <= '9') { // Check if character is a valid numeric digit
                board[r][c] = ch - '0'; // Convert character to integer and store in working grid
                initial_board[r][c] = ch - '0'; // Convert character to integer and store in initial grid
            } else { // Handle any other whitespace or formatting characters
                continue; // Skip current iteration for non-relevant characters
            } // End of character type check if-else block
            c++; // Increment column index counter
            if (c == GRID_SIZE) { // Check if end of row has been reached
                c = 0; // Reset column index to 0
                r++; // Increment row index to move to next row
            } // End of row boundary wrap if block
        } // End of file reading while loop
        file.close(); // Close the input file stream
        return true; // Return true indicating successful puzzle data loading
    } // End of loadPuzzle method

    void displayInitial(const string& filename) const { // Method to display initial file context and board
        cout << "Puzzle file name: " << filename << endl; // Output requested file name
        cout << "Initial Puzzle:" << endl; // Output header label for initial board state
        printBoard(initial_board); // Render the initial puzzle board configuration
    } // End of displayInitial method

    void runSolver() { // Method to trigger solving execution and report status
        solution_count = 0; // Reset solution counter prior to solving run
        solve(0, 0); // Initiate recursive backtracking solver at top-left cell (0,0)
        if (solution_count == 0) { // Check if no solutions were found during search
            cout << "No solution found" << endl; // Output required assignment no-solution message
        } // End of solution check if block
    } // End of runSolver method
}; // End of SudokuPuzzle class definition

int main(int argc, char* argv[]) { // Main application entry point supporting command-line parameters
    vector<string> filenames; // Dynamic vector to store file paths to process

    if (argc > 1) { // Check if command-line arguments were provided by user
        for (int i = 1; i < argc; i++) { // Loop through each argument provided
            filenames.push_back(argv[i]); // Add each filename to the processing queue
        } // End of argument collection loop
    } else { // Fallback if no command-line arguments were supplied
        cout << "Enter Sudoku puzzle file names (press Enter on empty line to finish):\n"; // Prompt user
        string fname; // Temporary string for console input
        while (true) { // Interactive file input loop
            cout << "File: "; // Print prompt prefix
            if (!getline(cin, fname) || fname.empty()) { // Check for empty entry or stream termination
                break; // Exit interactive loop
            } // End of input check if block
            filenames.push_back(fname); // Store entered filename
        } // End of interactive loop
    } // End of argument handling if-else block

    if (filenames.empty()) { // Check if filename list is empty
        cout << "No puzzle files provided." << endl; // Notify user that no work was specified
        return 0; // Terminate execution normally
    } // End of empty filename check if block

    for (size_t i = 0; i < filenames.size(); i++) { // Loop through each collected puzzle filename
        SudokuPuzzle puzzle; // Instantiate a distinct SudokuPuzzle object per file
        if (puzzle.loadPuzzle(filenames[i])) { // Check if file successfully loaded
            puzzle.displayInitial(filenames[i]); // Display file name and initial puzzle state
            puzzle.runSolver(); // Execute solver and output results
            cout << "----------------------------------------" << endl; // Print section divider
        } // End of file load success check if block
    } // End of batch file processing loop

    return 0; // Return exit code 0 indicating successful program completion
} // End of main function