-- choose your browser, (if you have it installed!)
--browser = "firefox"
--browser = "opera"
local browser = "iexplore"
local site = "http://www.myoddweb.com";

-- we need to know the number of arguments.
am_say( "Going to piger homepage ...", 400, 10 );
am_execute( browser, site );
