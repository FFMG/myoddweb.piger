-- Retrieve the drive letter,
function getdrive(p)
  local ch1 = p:sub(1,2)
  local ch2 = p:sub(2,2)
  if ch2 == ":" then
    return ch1
  end
  return "%HOMEDRIVE%";
end

-- do we have a folder?
local thisFolder =  am_getfolder( 0, false );

-- if we don't have a folder, guess what we are looking for.
if thisFolder == false  then
  local sizeOf = am_getCommandCount();
  if sizeOf == 0  then
    -- no arguments.
    am_execute( "cmd.exe", "/k \"title Piger Rules!!&&@cd %HOMEDRIVE%&&%HOMEDRIVE%\"", true );
  else
    local thisWord = am_getCommand( 1 );
    if thisWord == "home" or thisWord == "h" then
      am_say( "<i>Going to <b>home</b> drive</i>.", 400, 10  )
      am_execute( "cmd.exe", "/k \"title Piger Rules!!&&@cd %HOMEDRIVE%\\&&%HOMEDRIVE%\"", true );
    elseif thisWord == "system" or thisWord == "s" then
      am_say( "<i>Going to <b>system</b> drive</i>.", 400, 10  )
      am_execute( "cmd.exe", "/k \"title Piger Rules!!&&@cd %SYSTEMDRIVE%\\&&%SYSTEMDRIVE%\"", true );
    else
      am_execute( "cmd.exe", "/k \"title Piger Rules!!&&echo Unknown command:" .. thisWord .. "\"", true );
    end
  end
else
  -- go to the selected folder and in case of multiple drives, change the drive letter.
  am_execute( "cmd.exe", "/k \"title Piger Rules!!&&cd \"" .. thisFolder .. "\"\"&&" .. getdrive(thisFolder), true);
end