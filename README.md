# Git, C, CMake and State Machine Tutorial

## Getting Started

To do this tutorial you must fork this by clicking the fork button in the GitHub web interface. The instructions will always appear in the root folder of the repo and can be viewed in the web interface.

## Welcome

Hello and welcome to this Git tutorial. I am to help you get some savvy Git skills whilst also using the C compilation process and a C project as a means to provide a reasoning behind the power and necessity of Git. The C projects will also demonstrate how a state machine works and a basic way of implementing one.

Please note that if you're reading these instruction whilst doing the tutorial they may dissapear if you change branch or do some git trickery that causes them to move/change. As such, while you get to grips with Git I would reccomend moving these README.md files, as they become available, somewhere outside of the current Git repo while you complete the challenge.

## Git

So what is Git?

I hope you all already know the answer to this. Git is a version control software that is now more or less the gold standard in code version control used around the world. As such, knowing Git is a very important skill in today's workplace for anyone that has anything to do with the development of code, regardless of the code type. It can be useful not just code projects but for the version control of any sort of project based around non-binary files, such as PCB designs.

So without writing a big blurb on Git, I say we just get into it and learn by doing.

## What is Forking?

So you will of now forked my repo from GitHub. What this means is that you have more or less copied (not cloned) my repo. It is a complete copy, including the Git histroy, Git objects etc. The key point is that it now belongs to you and you can do as you wish with it, without disturbing my original repo. It is a handy feature that is implemented by the Git server systems (such as GitLab and GitHub), it is not intrinsically a part of Git. It allows for developers to essentially snapshot someone's project, modify it and then, if they want, they can send the changes back to the original developer who can decide if they want to merge their changes into the original code. Forking is popular in the open source community where people are not always directly included in someone's project.

Throughout this tutorial we will do two things, we will set up a new repository for you to use throughout the semester to manage your code and we will undertake some little challenges in this repository to help further your Git and C abilities.

## Basics

Now the very basics of Git. Git works using repositories (previously mentioned repos). Repos are essentially a mini filesystem (locally seen as a folder) in which all changes, that happen within that folder, are (or can be) tracked. Git monitors changes to files and stores snapshots of the files at specified points. By snapshotting the file system, Git gives each point point in the repo's history a unique value. Enabling a developer to revert changes, go back and look at previous, or parallel, version as well as lots of other cool stuff we will cover a bit more later.

Without going into it too much now, we know that a Git repo will give us a folder, whose content will be tracked. So now let's set up the repo that you will use throughout semester.

For anyone doing this that is not one of my students you will have to just ignore the specifics aimed at my students.

You can either use the LRZ GitLab or a **private** GitHub repo for this, the functionality is identical. Do not use a public GitHub as other students will be able to view your code and you might be involved in some nasty business if duplicate code is found in another student's project.

So now you will need to create a new project. This will require you to do the following:

1. Create a new project/repository in the web GUI
2. Either follow the given instructions to clone the new repository or copy the clone link so you can clone the repository to your machine which I will explain now.

  There are two types of links always available, one via SSH and the other via HTTPS. SSH allows you to upload your public SSH key to the server and use it to authenticate yourself when interacting with the server, meaning you do not have to constantly enter your username and password when interacting with the server. In contrast HTTPS requires you to enter your username and password in the terminal every time you interact with the server when authentication is required.

  I recommend uploading a SSH key and using the SSH link. Google how to do this, its not hard.

  Then to clone the repository you simply have to run the `git clone` command. Looking at the manual page for the clone command (`man git clone`) we can see that the command takes the form of `git clone <options> <repository> <directory>`. For us we will just use the basic command to clone our newly created repository into a folder called "ESPL_code". From the folder where you would like to have your `ESPL_code` folder stored run

  ``` bash
  git clone <SSH or HTTPS URL> ESPL_code
  ```

  Now we will have the Git repo on our local machines. Next we will look into the basics of creating and storing code in our repository. Please note that after the basics of the "ESPL_code" repository are set up we will do the rest of this tutorial in the tutorial repository, I am just wanting to make sure you have a Git repo set up and ready for the rest of semester so that you USE IT!

# How Git Tracks Your Changes

Git works slightly differently to other version control software in that is uses a staging area to which a user adds files that he/she wishes to be tracked (changes recorded). As such Git does not automatically track all changes in a repository. It is up to the user to make sure that the files that they are interested in are added to the staging area, there are ways to wildcard add things but this can lead to very messy and unprofessional repos.

As such the workflow follows the flow of:

1. Working directory

  Here a developer can modify/create/delete files in their project, just do your thing....write your code etc.

2. Staging area

  Here are the files that the user has told Git are important and should be tracked. If a file is not here then Git will not care for the changes made to that file. Files are added to the staging area using the `git add` command. Use `man git add` for a more detailed overview. It's quite straight forward.

3. Repository

  Once files have been added to the staging area, using `git add`, they can then be committed to the repository. A commit represents a snapshot in the repo's file system. The difference between two commits as seen as a set of changes to a set of files, also synonymous to a patch (funnily enough a lot of modern patches come straight from Git commits). Also note that that your changes and repository are local and not automatically sent to your Git server, this must be done manually. As such one must commit changes to your repository and then push the commits to the remote server, but we will get to this a little later.

The benefit of having a staging area is that a user can create separate commits for different chunks of work. If you modified two separate files you could add each file to the staging area individually, commit the changes, thus creating a psudo patch for that file then add and commit the second file creating a separate patch for the second file. While this functionality will most likely not be that useful for you in ESPL, it is an important concept to grasp, especially for industry work where the generation of patches is very important. As you should not have random files contained within your patches, such as debug files, knowing how to control the contents of a commit is very important.

Storing changes to file in the staging area are done using the `git commit` command. See `man git commit` for details. We will see how this is done in practice shortly and it should become clear if it's not making complete sense at the moment.

## What is a Commit?

Now that we know the rough workflow of Git, I feel that it will help to make the entire process of Git and tracking changes clearer if what a commit is is outlined.

A commit holds the state of the repository at the moment when the commit was created. This state is just a node in a linked list of the repository's previous states, with the commit node's parent(s) being the commits that came before it during the development within the Git repository. Git does not store the delta of each file like other versioning control software but instead creates copies of changed files, unchanged files are thus able to be simply referenced, such that that unnecessary copies of unchanged file are not made. This allows the user to navigate through this linked list of commits to look at the repository's snapshots at different points in time. As such, Git is more or less a mini filesystem that has some powerful tooling to achieve some pretty nifty tricks.

## Lets Commit!

Now let's put this into practice. Git repositories almost always contain a README.md file. The README is responsible for giving an overview of and/or instructions for the project on the GitHub/GitLab page when one views the repository in the web interface, as you are probably doing now.

The README usually contains useful information outlining the project, how to build it, developer's contact information etc. As such, we will create and add one to our newly created ESPL repository. Please then create, in your ESPL repository, a `README.md` file. The `.md` signifys that it is a markdown fil, allowing for markdown formatting of the text. Markdown formatting information can be found [here](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#links).

Please add your name and a quick description to the README then we will go through and add it.

``` bash
echo "# Your name here" > README.md
echo "A quick description" >> README.md
```

This will create the file and add your name and a description to the file, with your name being a H1 title.

Every time you make a change to the contents of your Git repository you can see the status of the repository using the command `git status`.

Running that now will show us a few things

```
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	README.md

nothing added to commit but untracked files present (use "git add" to track)
```

We can see that we are on our Master branch (we will cover branches a little later) and that we have the file `README.md` as an untracked file, meaning it is not in the staging area and would not be included in any commits.

Let's for testing purposes try and commit our README without adding it to the staging area. Looking at the manual for the `git commit` command (running `man git commit`). We can see the commands which can be used to add and remove files from the staging area (read the description section). Now a commit always requires a commit message, given using the `-m` option. Commit messages should always be precise and describe what changes have been implemented in this commit. Think of the commit message as a title/description for the patch that would be generated from the commit. As such to "attempt" to commit our README without adding it to the staging area we would run

``` bash
git commit -m "Added README"
```

We will now see that Git complains that nothing was added to the commit, meaning the staging area was empty. As we should all know by now, we need to add the `README.md` to the staging area. To do this we can either add the individual file or add all files in the repository. I would recommend avoiding adding all file where possible as you tend to include a lot of junk. We will cover how to avoid this a little later on.

For now add your README using

``` bash
git add README.md
```

Now attempting to commit should result in the output

```
[master (root-commit) 3c2b284] Added README
 1 file changed, 2 insertions(+)
 create mode 100644 README.md
```

Showing us a few things. It shows us our branch `master`, the beginning of the commit hash `3c2b284`, the commit message `Added README`, the changes made as well as the files added, in this case the `README.md`. To find out what the mode is read [here](https://stackoverflow.com/questions/737673/how-to-read-the-mode-field-of-git-ls-trees-output/8347325#8347325).

We have now successfully created a commit in our repository. Running `git log` we can see that the commit now appears in the repository's logs. In the log you can also see the entire commit hash which is used to identify that specific commit within your repo.

In a repo you will create many commits as you implement features and commit them, the commits will not be automatically visible to others who also have the repository on their machines or are looking through the web interface. Git does not automatically sync changes as it is designed to be usable offline, only syncing when told to. To then send your commits to the remote repository, stored on a Git server, you must `git push`. To understand what we are doing exactly when we are pushing we need to know a couple of things.

To push our code to the Git server we us the command `git push origin master`.

Breaking this command down we have:

 * `git push`, this is the command the tells Git to send all of the commits saved locally on your machine to the remote server.
 * `origin`, is the alias given to the remote server where our repository is stored. If you look into the `.git/config` file in your repositories root directory you will see something similar to the following

  ```
  ...                                                 
  [remote "origin"]                                                                
    url = git@git.alxhoff.com:alxhoff/espl-test.git                              
    fetch = +refs/heads/*:refs/remotes/origin/*                                   
  ...
  ```

  This is telling us that when we use `origin` we will be sending our commits to the server specified by that alias. It is possible to send to multiple servers at once, for instance a backup server, or to send to both a private and public server.

* `master`, tells Git which branch's commits we are wanting to send. We will cover branches soon.

Running `git push origin master` will show us the following

```
Enumerating objects: 3, done.
Counting objects: 100% (3/3), done.
Writing objects: 100% (3/3), 253 bytes | 253.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To git.alxhoff.com:alxhoff/espl-test.git
 * [new branch]      master -> master
```

which should be quick self-explanatory.

Now that we have covered how to add, commit and push we will stop using the ESPL repository and focus purely on this tutorial repository that you forked. The ESPL repository is there for you to use over the semester, read the exercise and project descriptions to see how your use of Git will be assessed. Using Git is required as part of the grading in this course, USE IT!

# Branching

Inside this repository there are a couple small programming challenge that we will get to soon. Through them where you will learn about some more advanced features of Git, the basics of building a C project and also do some very basic programming exercises. The first step will be to start our C project. To do this you will need to get and merge the CMake project to your master branch, to do this we need to learn about branches and merging. First step it branching.

A Git branch essentially takes a copy of the current state of your git repository and allows you to change it seperate to the branch from which you branch. Confused?
As the same implied, branching allows your to diverge your code base into a seperate "copy" that can be modified and later merged back into "core" of your repository.
Git uses branches to allow for parallel development of code without interfering with the code on other peoples machines or without breaking your stable code base whilst you hack around some changes. How exactly branches are used is a personal preference or the preference of your company, but there are some good ideas to employ when using your Git branches.

We have already come across the master branch, as the name implies this is the root branch of the repository and it usually the most sacred of all branches. Good practice is to not develop directly on the master branch as the master branch should always have a stable version of your code that builds and runs, while it might not have all the latest features, it should be able to be demo'd at any moment in time (except maybe exactly during a merge). You should as such never push to master, only merge to master. Be prepared for impromptu requests to see your code running, it should thus be able to be run from your master branch always even if it lacks all the latest features.

As master is this stable branch that must never break, you might of guessed that all development should be done on other branches. This is correct. When working in large groups or on a project where you are swapping between implementing multiple features at the same time, each person or feature should be a branch. I would recommend towards having a branch for a feature as multiple people might work on a feature over the duration of a project. To understand the next concept we need to have a basic understanding of what merging is, while we will look at merging in practice shortly, a theoretical understanding is needed.

Merging, as the name implies, is the task of merging the code (the changes) from one branch into another. If you have a branch to implement a certain feature, you would then merge this feature into your master branch once the feature is stable and completed. The merge process can become quite complex and involved but for now you should understand what merging aims to achieve. As you can imagine, merging large complex changes can become quite involved, while Git does a great job at handling most changes automatically it does not always manage on its own. As such, merging your changes into master can sometimes lead master to becoming unstable whilst the merge is handled. As such, you can imagine that employing a merge branch to handle the merging of complicated code changes into one single delta can act as a good intermediary between your feature branches and the master branch.

Therefore it can be good practice to have a branch structure similar to the following

```
master ----- merging ----- feature A
                     |---- feature B
```

Thus any complicated merge conflicts will be contained to the merging branch and can be resolved there before being merged (via a less complex merge, as the complex problems have now been resolved) into the master branch. We will cover merge errors and the likes later as your perform your own merges.

Let us now go to the merging branch. To check the branches that exist in the repository one can use the `git branch` command. A new repository might not show all of the branches as Git does not download all information when not required, to try and minimize the data required locally. `man git fetch` will detail how Git fetch can be used.

When no remote is specified the default `origin` is used. Run `git fetch` to thus retrieve all the branches and tags on the origin remote. Now run `git branch --all` to list all of the branches on the origin. We want a branch called `merging` where we will perform our merges before merging to master. To do this we need to first create a branch and then swap to this branch. To create a new branch simply use

``` bash
git branch merging
```
now if we list our branches using `git branch` you should now see that there is a `merging` branch. We now need to change to this branch so our modifications that we perform are done there. It should be noted that the new branch is a copy of this current branch, although if we were to continue modifying master then the merging branch would fall behind and would need to be brought back up to speed with master. But for now just checkout `merging` using the checkout command.

``` bash
git checkout merging
```
It is also good to note that this action of branching and checking out can be done in a single command by using the `-b` option with the `checkout` command.

``` bash
git checkout -b merging
```

# Merge Basics

Now that you have checked out your merging branch we are going to perform some merges. As this tutorial will also look into building C projects, using CMake specifically, we will using merging and other Git tools to pull a basic CMake project together.

Firstly we will want to make our Git server (origin remote) aware of this new branch we have created, as it does not get made aware of this change unless we tell it. Similarly to before we will use the `git push` command but this time our branch has changed.

As such please push the current branch using the previous command of

``` bash
git push ______ ______
```
You will need to fill in those two blanks.

Now back to the problem. You should be able to find a branch called `make`, check it out using your newly learnt checkout command. On this branch is the skeleton for our CMake project. Now to merge the `CMakeLists.txt` file, which is the core CMake file for any CMake build, into our merging branch. We need to use the `git merge` command. Details of this can be found in the manual, you should be able to run the correct `man` command yourself now to do this.

Merging is always done on the branch into which you wish to merge. If you wish to merge your `merging` branch into `master` you would first need to `git checkout master` and then merge `merging` into `master`. As we are wanting to merge the `make` branch into our current branch (merging) we don't need to change branches.

The `git merge` command handles the merging of files automatically, although it requires human intervention occasionally. We will get to this later. For now we simply want to merge the `CMakeLists.txt` into our current branch. As our branch does not have a `CMakeLists.txt` file the merge should not have any errors when performing this merge.

We can thus execute

``` bash
git merge make
```
This should give us an output along the lines of

``` bash
CMakeLists.txt | 12 ++++++++++++
1 file changed, 12 insertions(+)
create mode 100644 CMakeLists.txt
```
Telling us that a new file was created with 12 new insertions, 1 for each line in the file. Now if we run `git log` we will see the commits made on the make branch when this `CMakeLists.txt` file was added to the repo.

Now that we have got the commits from the make branch merged into our branch we should push these changes to the remote, running `git push` again will now show that the files have been pushed. If we rerun `git log` you will notice that the commit where the `CMakeLists.txt` file was commited has now changed from

``` bash
(HEAD -> merging, origin/make, make)
```
to
``` bash
(HEAD -> merging, origin/merging, origin/make, make)
```
meaning that this commit can now found be found in origin/merging and not just merging, origin/make and make. This annotation (`origin/`) signifies the remote branch (ie. the branch on the server). The branch merging is your local branch while the branch origin/merging is that on the remote.

# CMake

Now that we have merged our CMakeLists to our current branch we need to go about making the project build such that it is stable and is in a condition that we would be happy to have on `master`. Good practice when building code projects is to have a folder where all temporary and/or build files are kept such that your project folder doesn't become cluttered with temporary build files. Cleaning the build is also easier as all build files are clumped together.

A common standard practice is to use a `build` folder. As such create a build folder in your Git repo's root, such that the build folder and `CMakeLists.txt` are in the same folder.

Now running the command `man cmake` we can see that to execute a CMake script one simply has to call the command cmake and the path to the cmake script file. First navigate into the build directory and then execute `cmake ..` where `..` specifies the folder in which the CMake script can be found, while the current directory (build) is used as the build directory. Moving up a folder (`cd ..`) and running `git status` you can now see that the build directory is now untracked and has had changes done to it. Running `git status build` shows us that the build directory now includes a `CMakeCache.txt` and a directory `CMakeFiles`. These are the temporary files generated by CMake.

Now before we go ahead and actually get the CMake project building lets play it safe and add **all** of the current files in the Git to the staging area, commit and push them so that we have a safe point to return to. Do this yourself, using a meaningful commit message.

## .gitignore

When committing the files you will see a lot of new files being created in Git. These are all temporary build files and should not actually be added to Git. If you already was questioning what I was doing by adding all of these then pat yourself on the back, you were correct in thinking so. This is a common problem that people new to Git have in that they include all sorts of useless metadata files, build files and binaries to the Git repository so that they clutter the Git repository and make navigating around the branches difficult as you create little changes without meaning so that changing branches becomes more difficult. This will be something you will come across in the future. But for now we will now fix this error by using a file called the `.gitignore`.

The `.gitignore` is a hidden file that lives in the Git root and contains a list of files that should intentionally be left untracked. Meaning that changes to those files are not of concern to Git. A more detailed description of how to use this file can be found [here](https://git-scm.com/docs/gitignore).

For now we just want to tell Git that the build folder's contents should be left untracked. To do this we need to create the `.gitignore` file and put the build folder in it.

This can be done by runing
``` bash
echo "build/" > .gitignore
```
From the Git repo's root directory

## Removing Staging Cache

Now running Git status we can see that the `.gitignore` file is untracked but the files we wish to have untracked (the build folder) are still being tracked. This is because the files are in the staging cache and need to thus be removed before the gitignore will be applied to them. A common fix that is used is to simply remove all files from the staging cache and then add them back.

To do so run

``` bash
git rm -r --cached .
```
This will recursively remove tracked files from the staging cache. Running `git status` again will now show us that all of the files in the repo have been deleted, meaning deleted from the staging area. In the untracked files section you will now only see the README, CMakeLists and .gitignore as these files have not been ignored via the gitignore. Now we can add these files back and commit them using something such as "Actualizing gitignore" as the commit message. After pushing the new commit, if we look at the repo through the web interface, looking specifically at the files on the merging branch, you will see that the build files are not included. It is important to add all files that you do not want included in the repository to be added to the gitignore so that there is no way for them to become accidentally included in a commit, this makes you look like a Git noob if you are committing build files.


## Onwards With CMake

Now that we have solved that problem let us continue building our CMake project. Previously we saw that CMake complained that there were no sources given to the target foo. Let us pick through the CMake file so we understand what went wrong and what we need to fix it.

To do so jump to the branch `compiling`.

In your web browser, if you select the branch `compiling` you can read the README directly in the browser.
