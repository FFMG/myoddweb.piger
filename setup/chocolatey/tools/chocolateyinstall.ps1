$ErrorActionPreference = 'Stop'; # stop on all errors
$toolsDir   = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$filePath   = Get-Item "$toolsDir\piger-*.exe"

$packageArgs = @{
  packageName   = $env:ChocolateyPackageName
  fileType      = 'exe'
  file          = $filePath
  
  softwareName  = 'myoddweb.piger*' #part or all of the Display Name as you see it in Programs and Features. It should be enough to be unique

  checksum      = 'bbc9c8f6581bfce487731be0c3803578b5979459eaee5a2b9be64ca6c431bac0'
  checksumType  = 'sha256' #default is md5, can also be sha1, sha256 or sha512

  silentArgs   = '/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /SP-' # Inno Setup
}

Install-ChocolateyInstallPackage @packageArgs
