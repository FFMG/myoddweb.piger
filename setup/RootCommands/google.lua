-- choose your browser, (if you have it installed!)
--browser = "firefox"
--browser = "opera"
local browser = "iexplore"

-- we need to know the number of arguments.
local sizeOf = am_getCommandCount();
if sizeOf == 0  then
  -- try and get the word that might be highlighted.
  local word = am_getstring();
  if false == word then
    am_say( "Starting Google.", 400, 10 );
    am_execute( browser, wiki );
  else
    am_say( "Starting Google for : " .. word , 400, 10 );
    am_execute( browser, "http://www.google.com/search?hl=en&q=" .. word );
  end
else
  local query = ""
  local prettyQuery = "";
  for count = 1, sizeOf, 1  do
    -- the numbers are 0 based.
    -- and we ignore the first one as it is the command itself
    local word = am_getCommand( count )
    query = query .. word
    prettyQuery = prettyQuery .. "<b><i>" .. word .. "</i></b>"
    if count < sizeOf then
      query = query .. "+"
      prettyQuery = prettyQuery .. " and "
    end
  end

  am_say( "Searching Google for: " .. prettyQuery, 400, 10 );
  am_execute( browser, "http://www.google.com/search?hl=en&q=" .. query );
end