## Summary
How do I create packages? See https://chocolatey.org/docs/create-packages

## Create the nuget packet
### Prepare the files

- Edit the `myoddweb.piger.nuspec`
  - Set the version number
  - set the package Source Url to https://github.com/FFMG/myoddweb.piger
  - Update the release notes
  
- Edit the chocolateyinstall.ps1
  - Get the correct `checksum`
  
- Copy the exe
  - Copy the correct exe to the `tool\` folder and delete the old one.
  
### Pack the app
- Open an administrator command line into this directory
- Type `choco pack`
- Test the install

## Test
### The install
You might need to run the uninstall first.

Run `choco install myoddweb.piger -dv -s .`

### The uninstall

`choco uninstall myoddweb.piger -dv -s .`

# Push

in the folder where the nuspec was created....

set the apikey `choco apikey --key xxxxx-yyyyyy-zzzz --source https://push.chocolatey.org/`
you can get it from the account mage of chocolatey(https://chocolatey.org/account).

run `choco push myoddweb.piger.x.y.z.nupkg --source https://push.chocolatey.org/`, (make sure you change the name of the nupkg file name)
