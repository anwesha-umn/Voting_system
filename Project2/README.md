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

To clean the `src` directory of object files and the executable, type:

```sh
Project2/src $ make clean
```
Note: You must run `make clean` before recompilation, otherwise the Makefile will fail.  

All audit files generated from running the election app should appear within this directory.
