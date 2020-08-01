echo "Installing GitHub Release"
wget https://github.com/tcnksm/ghr/releases/download/v0.13.0/ghr_v0.13.0_linux_amd64.tar.gz
tar -xzf ghr_v0.13.0_linux_amd64.tar.gz
mkdir ~/bin
mv ghr_v0.13.0_linux_amd64/ghr ~/bin
source .profile

echo "Generating Release Tag"
chmod +x ./generate_release_tag.sh
VERSION=$(./generate_release_tag.sh)

echo "Publishing Release on GitHub"
ghr -t "${GITHUB_TOKEN}" \
	-u "${GITLAB_USER_NAME}" \
	-r "${CI_PROJECT_NAME}" \
	-c "${CI_COMMIT_SHA}" \
	-delete "${VERSION}" \
	./build/Release