
# csci5801_Project2 - Team4

## Project2 - Agile / Scrum - VotingSystem

### Team Members
- **Annabelle Coler** - coler018@umn.edu
- **Anwesha Samaddar** - samad037@umn.edu
- **Hilton Nguyen** - nguy4798@umn.edu
- **Zoe Sepersky** - seper011@umn.edu

## How to Compile the System

### Voting System Build / Clean Using Included Makefile
To build the voting system, navigate into the Project2 directory, go to the `src` folder and within the terminal type:

```sh
Project2/src $ 
```

Then, compile the project by typing `make`:

```sh
Project2/src $ make
```

This will create an executable file called `election_app` in the `src` directory. Run the project by typing:

```sh
Project2/src $ ./election_app
```

The screenshot below shows how the **PV election proceeds**, the display of results and the reporting of valid /invalid ballots, list of winners and losers and election stats. The user inputs the number of files, csv file names, and audit file name :
<p align="center">
<img src="https://github.umn.edu/umn-csci-5801-02-S25/repo-Team4/assets/32292/8c14935a-b5cf-4ca7-b263-dfaea5b639de" alt="PV Election" width="850"/>
</p>


The following screenshot shows the working of the **MV election** with all valid ballots:

<p align="center">
<img src="https://github.umn.edu/umn-csci-5801-02-S25/repo-Team4/assets/32292/8cdb9a97-7e1a-4e0f-bb67-75d6b8549e3b" alt="MV Election" width="850"/>
</p>

Finally, here is an example of running the **STV election** with valid and invalid ballots: 

<p align="center">
<img src="https://github.umn.edu/umn-csci-5801-02-S25/repo-Team4/assets/32292/c688040f-b3d9-464a-b27d-1f194f1a3169" alt="STV Election" width="850"/>
</p>

To clean the `src` directory of object files and the executable, type:

```sh
Project2/src $ make clean
```
Note: You must run `make clean` before recompilation, otherwise the Makefile will fail.  

### Unit Testing Files Build Using CMake
To build unit test files, ensure you are in the `testing` directory. Within CMakeLists.txt update the unit test file name within `add_executable()`, `target_link_libraries()` and `gtest_discover_tests()`.
Then type the following:

```sh
Project2/testing $ cmake -S . -B build
Project2/testing $ cmake --build build
Project2/testing $ cd build && ctest
```
Note: For running every unit test file we need to modify the CMakeLists.txt. There are some specific instructions for running stv_UT.cc (STV unit tests in the CMakeLists.txt within testing directory).
**IMPORTANT:** Tests that were written for PBIs 1-6 were written before the "read all information from the CSV file" functionality was implemented, and might have undefined behavior if ran.






