# cmpt360_lab9

# Usage:

./findme [starting directory] [options] <br>

Providing no options will print every file and providing no max depth will print to as far down the file structure as its defined. <br>

[options] <br>

-n, --name [a file name] will filter based on a given file name. <br>

-t, --type [f,d,s,c,b] include the type of file to be looked for; Regular file (f), Directory (d), Symbolic link (s), Character device (c), Block device (b) <br>

-u, --user [a username] in addition will match the owner of the file to be looked for <br>

-m, --maxdepth [amount of depth] limit how many directories search goes into <br>

Note: <br>

If as a user you are providing a --user or -u flag specifying to filter by a username, you need to provide a --name or -n file name in order for this to work. We ran into an efficiency error because we were calling ldapsearch for every file. So we had to resort to check if the file provided as a --name or -n flag and see if they match before checking if that file belongs to the user. Later implementations we may consider creating an array of structs to hold all the user information on the server and do a linear search in order to check if the file belongs to that given user. 
