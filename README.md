# lolef - A Cats take on a fuck you to Windows.
Same Privlege Escelation, new code:

The program first enables the debug privilege on the current process using `OpenProcessToken()` function. Then it opens the target process and gets its process token with `OpenProcessToken()` function.

The program duplicates the process token with `DuplicateTokenEx()` function to create a new token with the same privileges.

Finally, the program starts a new command prompt process with the new token using `CreateProcessWithTokenW()` function. This effectively runs the command prompt as SYSTEM.

The program cleans up by closing the handles to the new process, new token, process token, and process handle. It also frees the memory allocated for the command line argument.



Please do consider dropping some monies for Cat food.

Bitcoin: bc1qywm3pcgtwv2wx42ue9zelepdgukp4t94krh0va
