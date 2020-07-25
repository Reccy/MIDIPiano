#!/usr/bin/env bash

set -e

# Gets the current branch name on Git
function branch_name {
	#echo $(git rev-parse --abbrev-ref HEAD)
	echo "release/1.8"
}

# REGEX pattern for branch
function branch_pattern {
	p="s/\(release\)\/\([[:digit:]]\+\)\.\([[:digit:]]\+\)$/\\$1/"
	echo $p
}

# Parses the major branch name to get the major version
function get_major_version {
	echo $(branch_name) | sed "$(branch_pattern 2)"
}

# Parses the branch name to get the minor version
function get_minor_version {
	echo $(branch_name) | sed "$(branch_pattern 3)"
}

# Generates a tag name based on the current branch and passed in patch number
# Arg $1 -> The patch number
# Echo the tag
function generate_tag_with_patch_version {
	major_version=$(get_major_version)
	minor_version=$(get_minor_version)
	patch_version=$1

	echo "${major_version}.${minor_version}.${patch_version}"
}

# Checks if the tag exists in git
# Arg $1 -> The tag to check
# Echo 'true' -> The tag exists
# Echo 'false' -> The tag does not exist
function check_tag_exists {
	if [[ $(git tag -l "$1") ]]
	then
		echo 'true'
	else
		echo 'false'
	fi
}

function main {
	current_patch=0
	tagged=0

	while [ $tagged -ne 1 ]
	do
		tag=$(generate_tag_with_patch_version $current_patch)

		if [[ "$(check_tag_exists $tag)" = "true" ]]
		then
			current_patch=$((current_patch+1))
		else
			tagged=1
		fi
	done

	echo "$tag"
}

main
