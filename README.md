# GameOfLife

Game of Life Application

This is a Game of Life simulation, an interactive implementation of the famous cellular automaton devised by John Conway. The program allows you to create and evolve patterns on a grid based on simple rules that determine the survival or death of cells from one generation to the next. The app includes customizations for grid size, cell color, timer interval, and more, making it both educational and fun.
Features
Core Functionality

    Grid-Based Simulation: A grid of cells evolves over generations based on Conway's rules:
        Any live cell with fewer than two live neighbors dies (underpopulation).
        Any live cell with two or three live neighbors survives.
        Any live cell with more than three live neighbors dies (overpopulation).
        Any dead cell with exactly three live neighbors becomes a live cell (reproduction).

    Toroidal and Finite Modes:
        Toroidal mode wraps the edges of the grid, creating a seamless space.
        Finite mode enforces boundaries where cells on the edges do not interact with neighbors outside the grid.

Customization

    Settings Dialog:
        Modify grid size, timer interval, and cell colors (living and dead).
        Save and reset settings to defaults.
    Toolbar and Menu Options:
        Start, pause, or step through generations.
        Randomize the grid, with or without a seed for reproducibility.
        Import patterns or save/load the current grid as .cells files.
    Status Bar:
        Displays the current generation count and the number of living cells.

Advanced Features

    Pattern Importing: Load patterns from .cells files and center them on the grid.
    HUD Options:
        Show or hide gridlines.
        Toggle the neighbor count display.

Responsive Grid

    Automatically adjusts to different grid sizes.
    Provides immediate visual feedback for all actions.

Installation and Setup
Prerequisites

    C++ Development Environment: Ensure you have a compiler and IDE like Microsoft Visual Studio.
    wxWidgets: The application is built using the wxWidgets library. Install and configure wxWidgets before building.

Steps to Build and Run

    Clone or download the repository.
    Configure your development environment to include wxWidgets.
    Open the project in your IDE and build the solution.
    Run the executable to start the simulation.

How to Use
Main Window

    Toolbar Options:
        Play, pause, clear, or step through the simulation.
        Import new patterns or save/load existing ones.

    Grid Interaction:
        Click on cells to toggle their state (alive/dead).

Settings

    Access the Settings Dialog via the "Options > Change Settings" menu.
    Customize the following:
        Grid Size: Set the size of the grid (e.g., 5x5 to 100x100).
        Interval: Adjust the timer interval for automatic generation progression.
        Cell Colors: Choose colors for living and dead cells.

Menu Options

    File:
        Import, save, or create new game boards.
    Options:
        Randomize the grid or restore default settings.
    View:
        Show/hide neighbor counts, gridlines, or HUD.

Controls

    Play: Starts the simulation.
    Pause: Stops the simulation.
    Next: Moves forward by one generation.
    Clear: Resets the grid to all dead cells.
    Import: Loads a pattern from a .cells file.

Example Patterns

Try importing popular patterns like Gliders, Oscillators, or Still Lifes to see Conway's rules in action.
Credits

    Developed by: Roxythe (Rocky) Harding
    Inspired by John Conway's Game of Life.
    Built using wxWidgets for the GUI framework.

License

This project is licensed under the MIT License. See the LICENSE file for details.
