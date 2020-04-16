Now that you should have found the following:
- Library style code that exposes the function `num_to_words`
- A `.patch` file for CMake 

We will discuss what patches are as this will also help strengthen your understanding of commits as well as exposing you to what has the be the most important and legacy method for version control in the software industry. The patch will enable your CMake script to compile the `espl_lib` as a static library which we will then link into your project, using it's functions in your main function.

# Patches

A patch, is a record of the changes made to a set of files. Patch files can then be used to apply the changes to an unmodified sets of files. As such, Git uses patch style diffs (read `man diff` to understand this) to show the changes that have occurred in the repo since the last commit. Open the file where your main function is and add in another print statement directly below the one printing the "Hello ESPL" message. Now `git status` tells us that we have modified that one file but it does not tell us the changes. The command `git diff` will do this for us. You should have an output similar to

``` bash
diff --git a/src/main.c b/src/main.c
index 82e0743..cca099a 100644
--- a/src/main.c
+++ b/src/main.c
@@ -5,5 +5,6 @@
 int main()
 {
     printf("Hello ESPL");
+    printf("This is a new print");
    return 0;
 }
```

Now this is a Git diff. Git being the smart software it is, and as it was created by Linus Torvalds to maintain the Linux kernel, it makes sense that Git uses/creates patches to handle changes. As the Linux kernel uses email and patch files to handle developer submissions to the mainline kernel. What `git diff` produces is actually a unified format patch, read [here](https://en.wikipedia.org/wiki/Diff#Unified_format) for more info.

## Creating Patches

We can therefore create a unified format patch by simply saving the diff output to an appropriately named file. In this case we are patching the file where our main function lives, in my case this is `main.c` and as such I will name my match `main.patch`.

``` bash
git diff > main.patch
```

You should now have a patch file that contains the output of diff. Let's go through the pieces of the patch file so you can better understand it. Having a knowledge of patch files will be essential if you plan to work in any industry that deals with code bases that are handled by more than one person.

The first line tells us that we are dealing with a Git diff generated patch and the files that were changed was the main.c. The second line gives us the hashes of the before and after files as well as the mode of the given file, in this case an ordinary file. The following line shows the before and after file names, this is useful if a file changes its name or if a file is deleted (becomes `/dev/null`). Now comes the more important information. The line starting with `@@` tells us the statistics of the following diff hunk. A patch file can contain multiple of these. `@@ -5,5 +5,6 @@` has the format `@@ -<start line>,<number of lines> +<start line>,<number of lines> @@`. Below I have now included the modified file with line numbers.

``` C
 1 #include <stdio.h>                                                               
 2                                                                                  
 3 #include "espl_lib.h"                                                            
 4                                                                                  
 5 int main()                                                                       
 6 {                                                                                
 7     printf("Hello ESPL");                                                        
 8     printf("This is a new print");                                               
 9    return 0;                                                                     
10 }    
```
From the patch we can see that the modification starts at line 5, meaning the main function and the previous version contained 5 lines. The modified version also started at 5 but now contained 6 lines. The rest of the patch shows where the lines were added within that range, as we can see the line `+    printf("This is a new print");` was added, indicated by the +.

## Applying Patches

Now that we have our patch file we will want to try it out. To do this remove the line you added when generating your patch. Your file should now look the same as it did before. Running `git status` should show that you have a new untracked file which is your patch file but you `.c` file containing your main function should not be appearing as it has not changed, since you reverted the changes manually.

Now we have two ways to apply a patch, either running the patch hoping that the exact file names exist that the patch file specifies (which they do in our case) or you can apply the patch file to a specific file. Both methods are shown below. Pick one and apply your patch. `man patch` will give you more info on patching.

``` bash
patch < main.patch
```
or
``` bash
patch main.c < main.patch
```

Now you should be able to see your changes back in your `.c` file.

Now in this commit there is a `CMakeLists.patch` that contains a patch that will modify your pervious CMake script to build and link the `espl_lib` library you have either already found or need to find. Open the patch and go through it.

``` bash
CMakeLists.patch           
--- CMakeLists.txt  2019-03-20 10:25:46.547529531 +0100                      
+++ CMakeLists.txt  2019-03-20 10:28:36.907537321 +0100                          
@@ -5,8 +5,12 @@                                                                 
 set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)                  

 include_directories(include)                                                    
+include_directories(lib)                                                        

 file(GLOB SOURCES "src/*.c")                                                    

 add_executable(foo ${SOURCES})                                                  

+file(GLOB ESPL_LIB_SRCS "lib/*.c")                                              
+add_library(ESPL_LIB ${ESPL_LIB_SRCS})                                          
+target_link_libraries(foo ESPL_LIB)  
```

We can see that there has been 4 lines added. The first line has added the lib directory to our project's includes. This is such that the espl_lib.h can be found during building. The header file for a library exposes the libraries API, in this case it exposes the function `num_to_words` whilst keeping the inner workings of the library inaccessible to the programmer. As such the library ensures the programmer cannot call `get_digit_word` directly. This is good practice to employ. A `.c` and `.h` file combo should implement a black box of code. Your implemented functions inside the `.c` function don't necessarily need to be exposed via the `.h` file. The exposed functions should give as simple and easy to use interface as possible. Such that as much of the programming logic related to the "module" the files implement is maintained within the `.c` file, not necessarily being exposed via the `.h` if unnecessary. A common mistake of beginner programmers to no not properly modularize their code. You want to write you code such that you can make "modules" for different portions of you code, this way they are reusable and more easily tested as you can test them independently.

Now back to the patch. The three lines at the end of the patch handle the building and linking of the library to our executable. The first line should look familiar in that it is simply gathering the `.c` files from the lib folder needed to build the library. The next line tells CMake that it is to build a static library from the source files that were just globbed. During the build process you will see make compiling a separate target `ESPL_LIB` which will generate a library file (`.a`) in the build directory. The final command is quite self-explanatory and handles the linking of the library to the executable.

Now that we have our patch and understand it, continue gathering the library files and the `CMakeLists.patch` from this branch and merge them to merging. Once there you are to modify your main function to do the following (you should copy this so when you change branches you don't loose it).

# Coding Challenge II

We will now modify our existing code from coding challenge I to write a small program that does the following:

 * Prompt the user from the command line for a number input, instructions should be displayed
 * Call the `num_to_words` function from the `espl_lib` on the inputted numbers (look at the header file to see how this function should be called)
 * Display the returned string to the terminal
 * Prompt the user if they wish to either exit the program or run another number and handle their decision accordingly.

Unlike the first challenge, this challenge will make up part of the assessment for this tutorial.

So once your program is performing the above functions and is building using your patched CMake project in the merging branch. Merge it all to master and create a `git tag` (information on how to found [here](https://git-scm.com/book/en/v2/Git-Basics-Tagging)) with the annotation "Coding Challenge 2". This will be used as part of the exercise submissions.

In the rest of the tutorial you will create another little program that will also find itself ending up in your root folder with the same standard sub-directories, ie. src etc. Now we are using tags because they mark points in the repositories history (usually used for marking release versions of software) and as such when marking this tutorial I should be able to check out each tag and find the correct "release" responding to each challenge.

Following the completion of this go to the `conflicts` branch where we will look at how to deal with merge conflicts, merging the provided code to create a program that you will merge into the code for coding challenge 2, essentially replacing it. This will then be taged again.
