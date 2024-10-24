# ls - systems project 1
simple version of an ls style command listing the contents of a given directory.

takes a single argument, the argument is the path to a directory, either relative or absolute. if a directory is not provided, use the current working directory, i.e., ".", the dot directory.

for every entry in the directory (including hidden files, ., ..), it prints:
· name of the file in the directory
· number hard links to the file (from stat)
· type of file
· size of the file
· number of directory entries if the file itself is a directory 
· total size 
