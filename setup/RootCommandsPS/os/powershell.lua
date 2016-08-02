-- get the selected folder.
local thisFolder =  am_getfolder( 0, false );

-- if we have a folder, got to it.
if thisFolder == false  then
  am_execute( [[powershell.exe]], [[-noexit -command "cd ~"]], true);
else
  am_execute( [[powershell.exe]], [[-noexit -command "cd ']].. thisFolder ..[['"]], true);
end
