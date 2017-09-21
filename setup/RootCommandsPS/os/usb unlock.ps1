# Check if a give drive is locked or not.
function IsLocked( $drive )
{
  return (manage-bde -status $drive| Select-String -Pattern 'Lock Status').ToString().Split(":")[1].trim().ToLower() -eq "locked"
}

# keep track of what we unlocked
# so we can give a summary at the end.
$found = 0;

# Look for all the locked drives.
$drives = Get-WmiObject Win32_Volume -Filter ("DriveType={0}" -f [int][System.io.Drivetype]::removable)

# for each and every drives we found.
foreach( $drive in $drives )
{
  # is it locked?
  if( (IsLocked( $drive.DriveLetter )) -eq $true)
  {
    # add a message to say that we are unlocking this drive.
    [void]($am.Say( "Drive : "+$drive.DriveLetter +" is locked", 400, 10 ))

    # unlock it, (and ask for password)
    manage-bde -unlock $drive.DriveLetter -Password

    # track that we found a locked drive
    $found++;
  }

  # Move on to the next drive
}

# Give some sort of sumarry of what was done.
if( $found -eq 0 )
{
  # If we unlocked nothing just tell the user.
  [void]($am.Say( "Could not find any drives to unlock", 400, 10 ))
}
else
{
  # Otherwise just tell them how many we unlocked.
  [void]($am.Say( "Found : "+$found +" drive(s) locked", 400, 10 ))
}