# MIDIPiano
![Build and Test](https://github.com/Reccy/MIDIPiano/workflows/Build%20and%20Test/badge.svg) [![Reccy](https://img.shields.io/github/license/Reccy/MIDIPiano)](https://choosealicense.com/licenses/mit/)

Play and visualise music on the desktop.

*Currently not accepting contributions*

## Description
The purpose of this project is for me to learn
- C++
- CMake
- Test Driven Development
- CI
- OpenAL
- OpenGL
- Desktop App Programming

## Project Requirements
- ISO C++ 11
- CMake 3.1.0

## Branching Strategy
This project uses [semantic versioning](https://semver.org/).
This project uses a slightly modified version of the [GitLab Flow](https://docs.gitlab.com/ee/topics/gitlab_flow.html) model for managing branching.
The only change to the GitLab Flow model on this repo is that release branches also include the patch version, instead of just cherry picking onto the minor version.

### New Release
Branching off of master with the following format `release/MAJOR.MINOR.PATCH` will trigger a deploy and will automatically tag the latest commit with that version.

### Patch Release
To patch an existing release, branch off of the release branch and bump the patch version.
For example, if you want to patch `release/1.2.0`, you would:
- Merge bugfixes into `master`
- Branch `release/1.2.0` to `release/1.2.1`
- Cherry-pick the fixes from `master` into `release/1.2.1`
- Push `release/1.2.1` to the repo, triggering the pipeline

It is important to ensure that the bugfixes are in master so that later releases also contain the fix, and the bug does not resurface.
