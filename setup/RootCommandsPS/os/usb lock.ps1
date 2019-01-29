# Check if a give drive is locked or not.
function IsLocked( $drive )
{
  return (manage-bde -status $drive| Select-String -Pattern 'Lock Status').ToString().Split(":")[1].trim().ToLower() -eq "locked"
}

# keep track of what we locked
# so we can give a summary at the end.
$found = 0;

# Look for all the unlocked drives.
$drives = Get-WmiObject Win32_Volume -Filter ("DriveType={0}" -f [int][System.io.Drivetype]::removable)

# for each and every drives we found.
foreach( $drive in $drives )
{
  # is it locked?
  if( (IsLocked( $drive.DriveLetter )) -eq $false)
  {
    # add a message to say that we are locking this drive.
    [void]($am.Say( "Drive : "+$drive.DriveLetter +" is unlocked", 400, 10 ))

    # lock it
    manage-bde -lock $drive.DriveLetter

    # track that we found an unlocked drive
    $found++;
  }

  # Move on to the next drive
}

# Give some sort of sumarry of what was done.
if( $found -eq 0 )
{
  # If we locked nothing just tell the user.
  [void]($am.Say( "Could not find any drives to lock", 400, 10 ))
}
else
{
  # Otherwise just tell them how many we locked.
  [void]($am.Say( "Found : "+$found +" drive(s) to lock", 400, 10 ))
}