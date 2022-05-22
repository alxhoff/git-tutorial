# Compiling C programs

The compiling of a C program takes place in three steps. Each instrumented by CMake. In the following couple of exercises you will look at each step and how they work. This will hopefully mean I have less questions to answer later in the semester when you get a linker error.

The three steps are:
1. Preprocessing<br>
  Handling of preprocessor directives to produce completed C files for compilation.
2. Compilation<br>
  The creation of object files from C files.
3. Linking<br>
  The compiled object files are linked together to form a binary file that represents the built program.

## Preprocessor

A big advantage that the C programming language has over interpreter based languages, such as Python, as well as other compiled languages, such as Java, is its precompiler. This preprocessor allows a programmer to use certain directives to simplify coding. The most basic analogy for what a lot of the preprocessor does can be thought of as "cut and paste", where chunks of code are pasted around to minimize the amount of manual code copying and/or search and replace that the programmer must perform.

There are specific preprocessor directives, such as `#include` and `#define` which you should all be familiar with. One can also create conditional statements that allow for the preprocessor to either include or not include code sections. This is done with the directives `#if`, `#ifdef` and `#ifndef`. A common example of these directives would be when performing "feature inclusion" via compiled flags.

``` C
#ifdef USE_AMAZING_FEATURE
device_t dev = amazing_device
#else
device_t dev = NULL
#endif
```

### Include and Define Directives

#### Includes

The include directive should be one that everyone is familiar with (if you don't know it this tutorial is going to be extra fun for you). This directive is found at the top of most C files. Includes enable the inclusions (funny that) of header files that contain required function *declarations* (function prototypes) and any other data that is required to interact with the API that the header file exposes from the relevant `.c` file, eg. constant values. This is achieved by the preprocessor pasting the contents of the header file wherever the relevant `#include <filename>` directive is found. Remember this when trying to debug include errors.

#### Defines

The define directive is used for text substitution, allowing for few nifty little tricks when writing C code. Firstly the `#define` allows the programmer to set a flag or give a certain text literal a value, which is then substituted during the preprocessing step of the build process. Commonly called a "hash define". This gives a few advantages, outlines below are the key, and most common, advantages.

 * Setting flags.

    Using the `ifdef` conditional preprocessor statements one can set flags. For example a "debug" mode could be enabled using a Boolean debug flag that then allows for the inclusion of debug code, such as print statements

    ``` C
    #define DEBUG_MA_CODEZ	1
    ...
    #if DEBUG_MA_CODEZ
    printf("Value waz %d", da_value);
    #endif
    ...
    ```
 * Human readability.

    By using hash defines instead of magic numbers (should be avoided whenever possible) code is able to be a more easily read as it will real more like actual text. This is important when sharing code or developing with others. Your code should read like a book.

    ``` C
    #define SMILEY_X_LOCATION   12
    #define SMILEY_Y_LOCATION   34
    void function_that_draws_smiley ( int x_position, int y_position ) {
      ...
    }
    ...
    void main( int argc, char **argv ) {
        ...
        function_that_draws_smiley( SMILEY_X_LOCATION, SMILEY_Y_LOCATION);
        return 0;
    }
    ```
 * Centralizes frequently used variable values in one locations, making changing widely used value very easy.

    For example, if the smiley drawn in the previous example is drawn many times in your program and you need to shift its location a hash define allows for the smiley to be moved for all cases where its location's value is used. By changing one centralized value. Simply put, hash defined are slightly better magic numbers. A good example of this would be using the following

    ``` C
    #define PI   3.14
    ```
    should you decide that PI is better as 3.15 then you would be quickly and easily able to break your code by changing just one value :)

#### Macros

Now a more advanced use for defines is macros. Macros use the substitution of tokens with other tokens to allow for the generation of code. Macros are beneficial when the use of a function would be trivial and/or executionally slower.

  ``` C
#define CIRCLE_AREA(RADIUS)   (PI * RADIUS * RADIUS)
```
The use of the above macro produces more easily read code that is then evaluated into inline C code during precompilation. The precompiler would do the following, via substitution

  ```C
#define MY_CIRCLE_RADIUS  2
...
my_circle_area = CIRCLE_AREA(MY_CIRCLE_RADIUS)
```
Would become
``` C
my_circle_area = 3.14 * 2 * 2
```
To see some much more involved macros that are used in the auto generation of data structures in a state machine, look [here](https://github.com/alxhoff/Application_Interfacing_Governor/blob/master/AI_gov_phases.c).

## Compilation

Now the compilation of code is only one step in the process of building a program. After each C file is run through the preprocessor, so that all the preprocessor directives have been handled, the completed C files must then compiled into assembly code. This is done using a compiler, such as the GNU C Compiler (GCC). The assembly code must then be made into machine code that is actually usable by the target device. This is done by the assembler, which creates machine code from the assembly code. By modifying these pieces of software, collectively called a toolchain, you can change how the code is compiled. Various toolchains are available for different hardware architectures, such that the compiled code is compiled with regards to how the code executes on the target architecture. Compiling for a different architecture, such as compiling on an x86 laptop for a ARM based micro-controller, is known as cross compiling.

The file produced by the assembler is a binary file, the formats can vary (ELF, a.out, ...) but the idea is the same. The binary files are called objects, each object file contains the compiled code and exposes the symbols contained within the binary code. Symbols are the addresses of the variables and functions contained within a binary, they can be thought of as the "API" to the object file. The addresses of all the symbols within an object file are stored within a symbol table.

At this point all of your source files have been compiled into separate objects which are unaware of each other. Function calls to functions found in other object files are at this point pointing to an unknown memory locations, in an unknown binary file. These calls are stored as references to undefined addresses which are to be resolved in the last step of building a program, linking. If the completed program binary is to be able to execute, each function call needs to know where the pointed to code actually resides in memory. This is done by the third step in building a program, linking. Common errors pop up along the lines of "undefined reference" when a symbol cannot be resolved, meaning that the linking step was not able to be completed.

I should also mention that libraries, with which you are hopefully familiar with, are simply object files that can be reused by multiple programs such that the same code is not rewritten and/or recompiled unnecessarily.

## Linking

So we know now that our program is compiled into a number of object files (binary blobs) we need to resolve all of the references to the symbols that are pointed to and found in other object files. As the name implies, linking is the process of linking these references to the actual memory addresses in the appropriate object files. Linking to files within the same source code chunk is usually not that error prone. Most errors come from having to link against static libraries or source code found else where on a system or in another project. Common errors will be those that complain of either missing definitions (cannot find appropriate symbols to link against) or duplicate errors (multiple objects exposing the same symbol).

# CMake

So now that I have outlined a bit for you what happens when building a program, we will go through and build a demo program by using a program called CMake. CMake just helps in automating the build process a little, while not abstracting the process so much that one looses touch with what is happening. Using your newly discovered Git skills we will piece together a basic project and get it compiled and linked. You may have to apply some Git trickery along the way to get all the pieces ;).

So now that you have an idea of what goes into building a C project lets see how you can accomplish this using CMake. If we open the CMake file in this branch you should see the following

``` bash                                                         
project(git_tutorial)                                                            
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)                                  

set(CMAKE_BUILD_TYPE Debug)                                                      
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)                   

include_directories(include)                                                     

file(GLOB SOURCES "src/*.c")                                                     

add_executable(foo ${SOURCES})    
```

Now the first few lines should be quite self-explanatory. On the fourth line we set the output directory for our binary. Meaning we will need to create a `bin` directory in our repo's root. Do this now. The fifth line is where we tell CMake which directories we want to use as include directories. In the directories here is where make will then look for files that you include using `#include`. This CMake line equates then to the `-I` make build option. Another good common practice is to separate your `.c` source files from your `.h` source files. As can be seen in the CMake, we use a folder called `include` for the header files and a folder called `src` for the source files. Create the `include` folder so it sits alongside the bin and build directories. Following this we can see the `file` command. The file command is a file manipulation command used to achieve lots of different things. In this case it is used with `GLOB`. Glob is used to generate a list of files given one or more globbing expressions, storing the results in a variable, in this case `SOURCES`. From the provided globbing expression we can see that in this case the file command generates a list of all `.c` files that can be found in the `src` folder. Since we do not have this you will also need to create it.

The final line in the CMake lets CMake know that an executable should be built from the provided source files, in this case those stored in the `SOURCES` variable. This will also perform the required linking should there be more than one source file found, as there are going to be no object files external to our source files' directory.

Now we are almost there. We have the folder structure set up for our project but we are missing some code! We want to merge the new folder structures that we created back to our merging branch. The problem is that Git only tracks the contents of a folder and not folders themselves. As all of our folders are empty they would not be able to be tracked, commited and then merged to our `merging` branch. A convention of Git to keep Git directories that are empty are to add a `.gitkeep` file to them. This is a hack way of creating a reason for Git to track the folder. In this case we are in need of this folder structure for CMake and as such it is important that Git is able to track our folders. Let's touch a `.gitkeep` into each folder that is empty. `man touch` should help you understand what touching does.

Checking the status of your repo you should now see that you can track your empty `src`, `bin` and `include` directories as they all have `.gitkeep` files in them. Add and commit your folders with a meaningful commit message and then checkout your merging branch and merge the folders to your `merging` branch. Come back to this branch (`compiling`) when you have done that.

## Coding Challenge I

Now we are in need of the most the most fundamental of fundamentals to compile our project, a main function. As we will consider the main function a new "feature" that you are to add to your project you must perform the following steps:

 * Checkout a new branch to implement your features, branching from this branch.
 * Write a main function in an appropriate file in your project, that is to output "Hello ESPL" to the terminal.
 * Add, commit and PUSH the work you have done to the origin remote.
 * Merge the changes back to `merging`, pushing the result.
 * Come back to this branch to continue with the instructions below.

Now you should have at least one source file and be able to rerun your CMake command to generate the required make files for your project. If all went well in your build folder you should now see a Makefile that was generated from CMake. To then build your project you need to run the command `make` from within your build directory. If all went well you should now find a built binary in your bin directory with the same name as the executable specified in you `CMakeLists.txt`.

Navigate to your bin directory and execute the binary by using the `./` Linux convention. You should be greeted with "Hello ESPL".

Now we know roughly how to navigate around a Git repo, create, add and commit files as well as build a basic C project with an idea of what is happening behind the scenes. Before we look into the more advanced CMake features, such as linking libraries, you should merge your current project to master as it is now in a stable state. Binary files should not be included in your commits and should be left untracked. Modify your repository accordingly such the the binaries can never be accidentally added. Once that is done you will need to go to the `exercise` branch where you will need to use the Git logs, merging and the `git cherry-pick` command to get your exercise requirements and the library code. Useful articles can be found [here](https://www.git-tower.com/learn/git/faq/detached-head-when-checkout-commit) and [here](https://www.hacksparrow.com/how-to-merge-a-specific-commit-in-git.html) to explain the process.

One commit will contain the necessary `.h` and `.c` files for a static library that we will build called `espl_lib`. Another commit will contain a `.patch` for your CMake script to link the library. Checkout the commits, find the files, verify their contents and then use cherry pick to merge them to your merging branch. The instructions (in a separate `.md` file) to continue can also be found in a seperate commit.
