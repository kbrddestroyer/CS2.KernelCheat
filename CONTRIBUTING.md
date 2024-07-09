# Contributing [En]
## Introduction 

You can join my [Dev:: Playground](https://discord.com/invite/KCYk7z7msN) Discord server. 

## Getting a copy of a repository

First of all you need a **copy of this repository**. The best way to do it is *forking*.

## Commit requirements 

1. Branch naming
 Please note, that **main** is protected and you cannot push to this branch. To add your changes to **main** branch you need to complete some steps:
1.1 Creating a new branch
   **Branch naming** is also important. Repo structure requires *wip/* prefix for work-in-progress commits. Alternatively you can put your nickname there, I guess it's ok
   Next section describes the region you're editing. It's one of the following: *kernelmode, usermode, docs*.
   The last section is the name of the branch. So, for example, your branch name can be something like this: `wip/usermode/nullpointer-fix`
 1.2 Pushing changes. Make some changes, commit them and push.
   **Commit naming**. I'm trying to keep some system in commits, so please, before main commit message put the name of the region that you're edited in upper case (e.g. KERNELMODE)
 1.3 Opening a pull request to main
   Pull request triggers an MSBuild action. After all check you can safely merge

 ## Build trigger
 I think you've noticed **release** branch. You can open a pull request from **main** and after the merge a build action will be triggered. It automatically creates a tag and adds zipped artifacts. 

 ### This section will be changed
