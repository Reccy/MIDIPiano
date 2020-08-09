#!/usr/bin/env bash
LC_ALL=C

function log {
	printf "$1\n"
}

function fail {
	>&2 printf "$1\n"
	exit 1
}

function git_version_fail {
	local POSTFIX="Please ensure git 2.9 or greater is installed and is executable"
	fail "$1\n${POSTFIX}"
}

function validate_git_installed {
	git --version > /dev/null 2>&1

	if [[ $? != 0 ]];
	then
		git_version_fail "Could not invoke git"
	fi
}

function validate_git_version {
	log "Ensuring current git version is 2.9 or greater"

	validate_git_installed

	local VERSION_RAW=$(git --version)
	local REGEXP="^git version ([0-9]+).([0-9]+)"

	if [[ ! $VERSION_RAW =~ $REGEXP ]];
	then
		git_version_fail "Failed to get git version"
	fi

	local MAJOR=${BASH_REMATCH[1]}
	local MINOR=${BASH_REMATCH[2]}

	if [ "$MAJOR" -lt "2" ] || [ "$MINOR" -lt "9" ];
	then
		git_version_fail "Cannot continue setup: git version is $MAJOR.$MINOR"
	fi

	log "Continuing setup: git version is $MAJOR.$MINOR"
}

function setup_git_hooks {
	log "Setting up git hooks"

	local TARGET_HOOKS_PATH='.githooks'
	local CURRENT_HOOKS_PATH=$(git config --get core.hooksPath)

	if [[ $CURRENT_HOOKS_PATH == $TARGET_HOOKS_PATH ]];
	then
		log "Skipping as core.hooksPath already set to ${TARGET_HOOKS_PATH}"
	else
		log "Changing core.hooksPath to ${TARGET_HOOKS_PATH}"

		git config core.hooksPath ${TARGET_HOOKS_PATH}

		if [[ $? == 0 ]];
		then
			log "Finished git hooks setup"
		else
			fail "Failed to set core.hooksPath to ${TARGET_HOOKS_PATH}. Please ensure you have git version 2.9+ installed"
		fi
	fi
}

function run {
	log "Setting up local environment"
	validate_git_version
	setup_git_hooks
	log "Finished installing local environment"
}

run
