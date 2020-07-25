#!/usr/bin/env python
import subprocess
import re

class ReleaseBranchParser:
	# Branch Name Valid Values:
	# release/MAJOR.MINOR.PATCH					e.g. release/1.0.2
	# release/MAJOR.MINOR.PATCH/alphaVERSION	e.g. release/1.0.2/alpha1
	# release/MAJOR.MINOR.PATCH/betaVERSION		e.g. release/1.0.2/beta1
	# release/MAJOR.MINOR.PATCH/rcVERSION		e.g. release/1.0.2/rc1
	REGEX_BRANCH_PATTERN="release\/(\d+)\.(\d+)\.(\d+)(\/(alpha|beta|rc)(\d+)$|$)"

	def __init__(self, branch_name):
		self._branch_name = branch_name
		self._regexp = re.compile(self.REGEX_BRANCH_PATTERN)
		
		matched = len(self._regexp.findall(self._branch_name))

		if not matched:
			raise ValueError("Branch name invalid")

		self._groups = self._regexp.findall(self._branch_name)[0]

	def _major_version(self):
		return self._groups[0]

	def _minor_version(self):
		return self._groups[1]

	def _patch_version(self):
		return self._groups[2]

	def _release_type(self):
		return self._groups[4]

	def _release_type_version(self):
		return self._groups[5]

	def _has_special_release_type(self):
		return self._groups[4] != ''

	def print_info(self):
		print("Major Version: %s" % (self._major_version()))
		print("Minor Version: %s" % (self._minor_version()))
		print("Patch Version: %s" % (self._patch_version()))

		if not self._has_special_release_type():
			print("Standard Release")
			return

		print("Release Type: %s" % (self._release_type()))
		print("Release Type Version: %s" % (self._release_type_version()))

	def tag(self):
		if self._has_special_release_type():
			return "%s.%s.%s-%s%s" % (self._major_version(), self._minor_version(), self._patch_version(), self._release_type(), self._release_type_version())
		
		return "%s.%s.%s" % (self._major_version(), self._minor_version(), self._patch_version())

def tag_commit(tag):
	print("Tagging commit: %s" % tag)
	subprocess.check_call("git tag %s" % tag)

def push_tag(tag):
	print("Pushing tag to remote")
	subprocess.check_call("git push origin %s" % tag)

def main():
	print("Tagging Release Commit")
	git_branch_name = subprocess.check_output('git rev-parse --abbrev-ref HEAD').decode('utf8')[0:-1]

	parser = ReleaseBranchParser(git_branch_name)
	parser.print_info()
	
	tag_commit(parser.tag())
	push_tag(parser.tag())

main()
