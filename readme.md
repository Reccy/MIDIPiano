# MIDIPiano

[Build and Test]https://github.com/Reccy/MIDIPiano/workflows/Build%20and%20Test/badge.svg)

[![Reccy](https://img.shields.io/github/license/Reccy/MIDIPiano)](https://choosealicense.com/licenses/mit/)

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
This project uses the [GitLab Flow](https://docs.gitlab.com/ee/topics/gitlab_flow.html) model for managing branching.

Branching off of master with the following format `release/MAJOR.MINOR` will trigger a deploy and will automatically tag the latest commit with that version.
Merging into a release branch again will bump the patch version and release a patch for that release.
