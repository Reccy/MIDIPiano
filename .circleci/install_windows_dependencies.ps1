if ( Test-Path $Env:ProgramFiles\CMake ) {
	Write-Output "Skipping CMake Install"
	Write-Output "Adding CMake to PATH"
	SETX /M PATH "$($Env:PATH);C:\Program Files\CMake"
} else {
	Write-Output "Installing CMake"
	choco install cmake -y --installargs 'ADD_CMAKE_TO_PATH=User'
}
