Write-Host "Installing CMake using Chocolatey"
choco install cmake -y --installargs 'ADD_CMAKE_TO_PATH=System'

Write-Host "Refreshing Powershell"
$env:ChocolateyInstall = Convert-Path "$((Get-Command choco).Path)\..\.."   
Import-Module "$env:ChocolateyInstall\helpers\chocolateyProfile.psm1"
refreshenv

Write-Host "Creating build directory"
mkdir build

Write-Host "Generating build system with CMake"
cd build
cmake ..

Write-Host "Building project in Release configuration"
cmake --build . --config Release