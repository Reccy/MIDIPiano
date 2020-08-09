# MIDIPiano
![Build and Test](https://github.com/Reccy/MIDIPiano/workflows/Build%20and%20Test/badge.svg) [![Reccy](https://img.shields.io/github/license/Reccy/MIDIPiano)](https://choosealicense.com/licenses/mit/)

Play and visualise music on the desktop.

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
- CMake 3.1.0 or greater
- git 2.9 or greater

## Local Setup
*Currently not accepting contributions*

To begin contributing to this project, first run the script `.install/install.sh`.

## Branching
### Branch Names
When creating a branch, you must reference it to a GitHub issue by the label and the issue ID.
This is done by following this format:
- `ISSUE_LABEL/ISSUE_ID`

Examples of possible branch names attached to issues are:
- `feature/57`
- `infrastructure/47`
- `doc/12`
- `bug/35`

Release branch names follow the [semantic versioning](https://semver.org/) format:
- `release/MAJOR.MINOR.PATCH`

For example:
- `release/1.33.0`
- `release/1.33.1`
- `release/1.33.2`
- `release/1.34.0`
- `release/2.0.0`

### Branching Strategy
This project uses a slightly modified version of the [GitLab Flow](https://docs.gitlab.com/ee/topics/gitlab_flow.html) model for managing branching.
The only difference is that release branches also include the patch version.

All commits will be squashed before merging into `master`.

### New Release
Branch off of `master` with the following format `release/MAJOR.MINOR.PATCH` to trigger a release GitHub Action workflow.
The workflow will automatically tag the latest commit with the version in the branch name. E.g. `v1.33.2`
The workflow will then build the project, run tests, and create a new release with a downloadable executable.

### Patch Release
To patch an existing release, branch off of the release branch and bump the patch version.
For example, if you want to patch `release/1.2.0`, you would:
- Merge bugfixes into `master`
- Branch `release/1.2.0` to `release/1.2.1`
- Cherry-pick the fixes from `master` into `release/1.2.1`
- Push `release/1.2.1` to the repo, triggering the pipeline

It is important to ensure that the bugfixes are in master so that later releases also contain the fix, and the bug does not resurface.
