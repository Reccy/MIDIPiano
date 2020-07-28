if ( Test-Path $Env:ProgramFiles\CMake ) {
	Write-Output "Skipping CMake Install"
	
	Write-Output "Adding CMake to PATH"
	$oldpath = (Get-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH).path
	$newpath = "$oldpath;C:\Program Files\CMake"
	Set-ItemProperty -Path 'Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment' -Name PATH -Value $newPath
} else {
	Write-Output "Installing CMake"
	choco install cmake -y --installargs 'ADD_CMAKE_TO_PATH=User'
}
