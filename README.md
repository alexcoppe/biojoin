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

Get the package from github and compile it using make

```console
>>> git https://github.com/alexcoppe/biojoin.git
>>> cd biojoin
>>> make
```

After compilation, move the generated executable ```biojoin++``` to a directory listed in the $PATH variable. You can identify these directories by using the ```echo $PATH``` command.

# Run the software :running_man:

## Table with all flags and arguments

Flags and Arguments | Type | What you get
------------ | ------------- | -------------
-h | None | show help options
-b | None | the first input file is a BED
-B | None | the second input file is a BED
-g | None | the first input file is a GTF
-f | String (example 0,3,5) | field from first file to be used as key
-s | String (example 2,3,8) |field from second file to be used as key
-d | Char (example ,) | The field separator string in the first file argument (default tab)
-e | Char (example ;) |The field separator string in the second file argument (default tab)
-o | Char (example ,) | The field separator the user wants in the output (default tab)



## Examples:

Below are some example usage commands using the files included in the data directory from the downloaded package.

### :computer: INNER JOIN on BED files

An **INNER JOIN** can be performed on two BED files using the standard genomic columns: chromosome, start, and end.  
When using the `-b` option (first file as BED) or the `-B` option (second file as BED), the `-f` and `-s` options are not allowed.  
In these cases, the join key is automatically constructed using the canonical BED fields: the first, second, and third columns of each row.


```console
biojoin++ -b -B data/type_of_gene.bed data/oncogene_tumorsupressor.bed

chr17	7668420	7677451	TP53	Protein	chr17	7668420	7677451	TP53	TSG
chr1	173863900	173867989	GAS5	LongNonCoding	chr1	173863900	173867989	GAS5	TSG
chr12	25205245	25250929	KRAS	Protein	chr12	25205245	25250929	KRAS	Oncogene
chr17	43044294	43125364	BRCA1	Protein	chr17	43044294	43125364	BRCA1	TSG
chr11	65497737	65506516	MALAT1	LongNonCoding	chr11	65497737	65506516	MALAT1	Oncogene
chr12	53962311	53974954	HOTAIR	Oncogene	chr12	53962311	53974954	HOTAIR	Oncogene
chr10	17228240	17237593	VIM	LongNonCoding	chr10	17228240	17237593	VIM	Oncogene
```

### :computer: INNER JOIN between a GTF and a BED file

Build the map from a GTF file, as indicated by the `-g` option and filter it using an **INNER JOIN** with `type_of_gene.bed`. The `-o` indicates the separator in the output. 

```console
biojoin++ -g -B  -o , data/oncogene_tumorsupressor.gtf   data/type_of_gene.bed

chr17,User,gene,7668421,7677451,.,.,.,gene_id "TP53"; gene_name "TP53"; type "TSG";,chr17,7668420,7677451,TP53,Protein
chr1,User,gene,173863901,173867989,.,.,.,gene_id "GAS5"; gene_name "GAS5"; type "TSG";,chr1,173863900,173867989,GAS5,LongNonCoding
chr12,User,gene,25205246,25250929,.,.,.,gene_id "KRAS"; gene_name "KRAS"; type "Oncogene";,chr12,25205245,25250929,KRAS,Protein
chr12,User,gene,53962312,53974954,.,.,.,gene_id "HOTAIR"; gene_name "HOTAIR"; type "Oncogene";,chr12,53962311,53974954,HOTAIR,Oncogene
chr10,User,gene,17228241,17237593,.,.,.,gene_id "VIM"; gene_name "VIM"; type "Oncogene";,chr10,17228240,17237593,VIM,LongNonCoding
```

An important thing to remember is that the start coordinate (column 4) of a GTF file is 1-based and inclusive. The end coordinate is also inclusive, representing the last base of the feature. See the Coordinate Rules section for a clearer explanation.

### :computer: INNER JOIN between a CSV file and a BED file

Build the map from a CSV gene file, which requires the `-d` option to specify the field separator of the first input file, and filter it using an **INNER JOIN** with `oncogene_tumorsuppressor.bed`, as indicated by the `-B` option.  
The `-f` option specifies the columns to be used from the first file to construct the JOIN key (note that column indexing for `-f` starts from `0`, not `1`).


```console
biojoin++ -d , -f 1,2,3 -B data/type_of_gene.csv data/oncogene_tumorsupressor.bed

TSG	chr17	7668420	7677451	SG	chr17	7668420	7677451	TP53	TSG
GAS5	chr1	173863900	173867989	TSG	chr1	173863900	173867989	GAS5	TSG
KRAS	chr12	25205245	25250929	Oncogene	chr12	25205245	25250929	KRAS	Oncogene
BRCA1	chr17	43044294	43125364	TSG	chr17	43044294	43125364	BRCA1	TSG
HOTAIR	chr12	53962311	53974954	Oncogene	chr12	53962311	53974954	HOTAIR	Oncogene
VIM	chr10	17228240	17237593	Oncogene	chr10	17228240	17237593	VIM	Oncogene
```

### :computer: CSV output

Same as the previous example, but the output is formatted as a CSV file by setting the output field separator with the `-o ,` option.


```console
biojoin++ -d , -f 1,2,3 -B -o , data/type_of_gene.csv data/oncogene_tumorsupressor.bed

TSG,chr17,7668420,7677451,SG,chr17,7668420,7677451,TP53,TSG
GAS5,chr1,173863900,173867989,TSG,chr1,173863900,173867989,GAS5,TSG
KRAS,chr12,25205245,25250929,Oncogene,chr12,25205245,25250929,KRAS,Oncogene
BRCA1,chr17,43044294,43125364,TSG,chr17,43044294,43125364,BRCA1,TSG
HOTAIR,chr12,53962311,53974954,Oncogene,chr12,53962311,53974954,HOTAIR,Oncogene
VIM,chr10,17228240,17237593,Oncogene,chr10,17228240,17237593,VIM,Oncogene
```

### :notebook: GTF vs. BED Coordinate Rules


| Feature | BED (Browser Extensible Data) | GTF (Gene Transfer Format) |
| :--- | :--- | :--- |
| **Coordinate System** | **0-based** (Starts counting at 0) | **1-based** (Starts counting at 1) |
| **Start Position** | **Exclusive** (The start coordinate is not included) | **Inclusive** (The start coordinate is included) |
| **End Position** | **Inclusive** | **Inclusive** |
| **Mathematical Length** | `End - Start` | `(End - Start) + 1` |
