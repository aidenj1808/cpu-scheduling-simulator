# CPU Scheduling Simulator

A simulator of CPU scheduling algorithms written in C, plus `findme`, our own version of the `find`
command. We built them for CMPT 360 at MacEwan University in winter 2024.

## CPU scheduler

`pSimulator` simulates processes arriving, waiting in a ready queue and taking turns on one CPU. It
steps through time in 0.1-unit ticks and implements three scheduling algorithms:

- First in, first out (FIFO)
- Shortest job first (SJF)
- Round robin (RR), with a 0.5-unit time slice

Each process is a text file in `pSimulator/newProc/` holding its run time and niceness, like
`4.00,3`. The simulator loads them into the ready queue, logs every tick to a CSV file in
`pSimulator/log/`, and `plotData` draws the log as a timeline of when each process ran, along with
the average response and turnaround times.

## findme

`tools/findme` searches a directory tree like `find`:

```sh
./findme [starting directory] [options]
```

- `-n`, `--name`: match a file name. Wildcards work if you put the name in double quotes.
- `-t`, `--type`: match a file type: regular file (`f`), directory (`d`), symbolic link (`s`),
  character device (`c`) or block device (`b`).
- `-u`, `--user`: match the file's owner. It needs `--name` too, and it looks users up with
  `ldapsearch`, so it only works on a network with an LDAP directory, like the university's.
- `-m`, `--maxdepth`: limit how many directories deep the search goes.

With no options, it prints every file under the starting directory. It's built with `getopt_long`,
POSIX threads and `glob`.

## Getting started

You'll need Linux with `gcc`, `make` and the cairo development package (`libcairo2-dev` on Debian
and Ubuntu). This is the code as we submitted it in 2024, built on the university's Linux servers,
so the makefiles may need small changes elsewhere.

To run the scheduler on 10 random processes and plot the result:

```sh
cd pSimulator/src
make test
```

That writes the log and the chart (`atest.png`) to `pSimulator/log/`. Pick the algorithm by
uncommenting the `ALGOR_FIFO`, `ALGOR_SJF` or `ALGOR_RR` line at the top of `procSim.c`. Run
`make clean` to clear the logs and process files.

To build `findme`:

```sh
cd tools/findme/src
make findme
```

## Project structure

- `pSimulator/src/dStruct.c`: the linked list the queues are built on.
- `pSimulator/src/procSim.c`: the simulator and its scheduling algorithms.
- `pSimulator/src/plotData.c`: draws the timeline with cairo.
- `pSimulator/src/files.sh` and `files2.sh`: create process files with fixed or random values.
- `tools/findme/src/findme.c`: the `find` recreation. `src/test/` is a folder tree to search.

## Built with

C, cairo, POSIX threads, make and Bash.

## Team

- Aiden Lumley ([@aidenj1808](https://github.com/aidenj1808))
- Taylor Carreiro ([@TayCarr](https://github.com/TayCarr))
