$ErrorActionPreference = 'Stop'; # stop on all errors
$packageArgs = @{
  packageName     = $env:ChocolateyPackageName
  softwareName    = 'myoddweb.piger*'
  fileType        = 'exe'
  silentArgs      = '/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /SP-' # Inno Setup
  validExitCodes  = @(0)
}

$uninstalled = $false
[array]$key = Get-UninstallRegistryKey -SoftwareName $packageArgs['softwareName']

if ($key.Count -eq 1) {
  $key | ForEach-Object {
    $file = "$($_.UninstallString.Trim('"'))"
 
  Uninstall-ChocolateyPackage `
    -PackageName $packageArgs['packageName'] `
    -FileType $packageArgs['fileType'] `
    -SilentArgs "$packageArgs['silentArgs']" `
    -ValidExitCodes $packageArgs['validExitCodes'] `
    -File "$file"
  }
} elseif ($key.Count -eq 0) {
  Write-Warning "$packageArgs['packageName'] has already been uninstalled by other means."
} elseif ($key.Count -gt 1) {
  Write-Warning "$($key.Count) matches found!"
  Write-Warning "To prevent accidental data loss, no programs will be uninstalled."
  Write-Warning "Please alert package maintainer the following keys were matched:"
  $key | % {Write-Warning "- $($_.DisplayName)"}
}

