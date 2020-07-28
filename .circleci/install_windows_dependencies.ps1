if ( Test-Path $Env:ProgramFiles\CMake ) {
	Write-Output "Skipping CMake Install"
} else {
	Write-Output "Installing CMake"
	choco install cmake -y --installargs 'ADD_CMAKE_TO_PATH=User'
}
