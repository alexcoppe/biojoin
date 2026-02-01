# biojoin++ :dna:

A command-line tool for joining biological data files (e.g., TSV, GFF, BED). This project is still under heavy development; currently, only BED and generic delimited files are supported

Its compilation and functionality have been verified on the following operating system:

- macOS :green_apple:
- Linux :penguin:

# Download and Compilation :floppy_disk:

This project is built using the C++23 standard. To compile and run the software, you need the make command and modern C++ compiler:

- GCC: Version 11.1+ (experimental support) or 14.1+ (recommended for full features).

- Apple Clang: Included with Xcode 15+ on macOS.

You can verify your current version by running one of the following commands in your terminal:

```console
# For GCC
g++ --version

# For Clang
clang++ --version
```

## Download and Compilation

```console
>>> git https://github.com/alexcoppe/biojoin.git
>>> cd biojoin
>>> make
```

After compilation, move the generated executable ```biojoin++``` to a directory listed in the $PATH variable. You can identify these directories by using the ```echo $PATH``` command.

# Run the software :running_man:

## Examples:

Example of flags for specifying desired fields:

Perform a left join on two BED files using standard genomic columns: chromosome, start, and end.

```console
biojoin++ -b -B data/type_of_gene.bed data/oncogene_tumorsupressor.bed
```

Perform a left join when the first file is a comma-separated values (CSV) file specified with -d , while the second (-B) is a standard BED file.

```console
biojoin++ -d , -f 1,2,3 -B data/type_of_gene.csv data/oncogene_tumorsupressor.bed
```

## Table with all flags and arguments

Flags and Arguments | Type | What you get
------------ | ------------- | -------------
-h | None | show help options
-b | None | the first input file is a BED
-B | None | the second input file is a BED 
-f | String (example 0,3,5) | field from first file to be used as key
-s | String (example 2,3,8) |field from second file to be used as key
-d | Char (example ,) | The field separator string in the first file argument (default tab)
-e | Char (example ;) |The field separator string in the second file argument (default tab)
-o | Char (example ,) | The field separator the user wants in the output (default tab)
