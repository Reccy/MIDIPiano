# MIDIPiano
![Build and Test](https://github.com/Reccy/MIDIPiano/workflows/Build%20and%20Test/badge.svg) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/ff87ca54e8b94a60b9b0329bffcd7992)](https://www.codacy.com/manual/Reccy/MIDIPiano?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Reccy/MIDIPiano&amp;utm_campaign=Badge_Grade) [![Reccy](https://img.shields.io/github/license/Reccy/MIDIPiano)](https://choosealicense.com/licenses/mit/)

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
- C++ 17
- CMake 3.15.0 or greater
- git 2.9 or greater

## Contributing
*Currently not accepting contributions*

### Setup
To setup the local environment for development on this project, first run the script `install/install.sh`.

### Documentation
When developing on this project, tracking code changes are done on 3 different levels:
- Pull Requests
- Commit Messages
- Changelog Entries

#### Pull Requests
Pull Request descriptions and comments should explain why you're changing the code in the pull request.

Ideally it should match the changes requested in the attached issue.  

Make sure that the last line of the Pull Request description closes the appropriate issue ID.  
E.g. `Closes #24`.

#### Commit Messages
Commit Messages should give a history to the work done on a branch.

They should describe what changed and be written in the present tense.  
E.g. `Change AudioClip::get_duration() return type to float`


When squashing commits to be merged to master, the title of the commit should match the issue name.

The squashed commit messages can then be referenced in the extended description of the message to describe all of the changes.

It's okay to delete commit messages that are no longer relevant in the description.  
E.g. `WIP 2 FINAL`

#### Changelog Entries
Changelog Entries should describe what behaviour changed in the app that is obvious to the user.

If it's something simple like a refactoring where no behaviour changed, then it's okay to just say `Refactor AudioClip class` and to link the appropriate ticket.

For something else like a bug or new feature, a longer description and a link to the issue is necessary.  
E.g. `Add mute audio keyboard shortcut`

### Pull Requests
Each Pull Request requires a `changelog.md` entry with the format:  
`-   Short summary of change (ticket-url)`

The entry should be placed under the `Unreleased` category.

### Releases
When creating a release, perform the following steps:
1. Update changelog on feature branch ([See Updating Changelog for Release](https://github.com/Reccy/MIDIPiano/tree/master#updating-changelog-for-release))
2. Merge pull request to master
3. Ensure CI pipeline passes on master
4. Create a release branch off of master ([See New Release](https://github.com/Reccy/MIDIPiano/tree/master#new-release))

#### Updating Changelog for Release
Change the `Unreleased` category in `changelog.md` to match the following format:  
`## \[MAJOR.MINOR.PATCH] - YYYY-MM-DD`

For example:  
`## \[1.2.12] - 2020-04-02`

This allows the build pipeline to generate the changelog for the GitHub Release page.

If there is no section in the changelog for that release branch on the correct release date *(in UTC)*, then the pipeline will intentionally fail.

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
