## Summary
How do I create packages? See https://chocolatey.org/docs/create-packages

## Create the nuget packet
### Prepare the files

- Edit the `myoddweb.piger.nuspec`
  - Set the version number
  - set the packageSourceUrl
  - Update the release notes
  
- Edit the chocolateyinstall.ps1
  - Get the correct `checksum`
  
- Copy the exe
  - Copy the correct exe to the `tool\` folder and delete the old one.
  
### Pack the app
- Open an administrator command line into this directory
- Type `choco pack`
- Test the install

## Test the install

`choco install myoddweb.piger -dv -s .`

`choco uninstall myoddweb.piger -dv -s .`
