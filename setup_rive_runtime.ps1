# PowerShell script to download and set up Rive C++ runtime

# Create directories if they don't exist
$riveCppDir = "rive-cpp"
$includeDir = "$riveCppDir/include"
$libDir = "$riveCppDir/lib"

New-Item -ItemType Directory -Force -Path $includeDir
New-Item -ItemType Directory -Force -Path $libDir

# Download Rive C++ runtime
$riveVersion = "0.9.0"
$riveUrl = "https://github.com/rive-app/rive-cpp/archive/refs/tags/v$riveVersion.zip"
$zipFile = "rive-cpp.zip"

Write-Host "Downloading Rive C++ runtime v$riveVersion..."
Invoke-WebRequest -Uri $riveUrl -OutFile $zipFile

# Extract the zip file
Write-Host "Extracting Rive C++ runtime..."
Expand-Archive -Path $zipFile -DestinationPath "temp" -Force

# Copy include files
Write-Host "Copying include files..."
Copy-Item -Path "temp/rive-cpp-$riveVersion/include/*" -Destination $includeDir -Recurse -Force

# Copy library files
Write-Host "Copying library files..."
Copy-Item -Path "temp/rive-cpp-$riveVersion/lib/*" -Destination $libDir -Recurse -Force

# Clean up
Write-Host "Cleaning up..."
Remove-Item -Path $zipFile -Force
Remove-Item -Path "temp" -Recurse -Force

Write-Host "Rive C++ runtime setup complete!" 