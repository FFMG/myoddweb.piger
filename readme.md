# Description

Piger is a Launcher allowing you to run your own commands from your keyboard. Highlight a word hold the caps lock key and simply type 'Google' to search for it in your browser. You can create your own commands, in LUA, C++ and more.

## Remark

Piger is aimed at people that like to write their own plugins, so by default it does not come with a whole lot of actions.

It is up to you, to create awesome plugins that fit **your** needs.

# Usage

Simple create your own actions/plugins and make them behave as you wish, for example, create a "google" command, (see below).

Then, after startup, simply press the [Caps Lock] button and type "go...", (you probably don't need to enter more than 2 letters), and google will pop up...

Of course, the main aim is for you to write your own plugins rather than been spoon-fed actions ...

# Special folders

## Root command
All your commands are located in your rootcommand folder, (%appdata%\myoddweb\ActionMonitor\RootCommands\ by default).

At statup all the actions/plugins are loaded. A file will have the command name.

For example, the file "**google.lua**" will create a command called "**google**" and the code in "google.lua" will be executed when that action is called. 

### Sub folders

All sub folders are parsed, a file in the folder "**\os\calc.lua**" will create a command "**calc**"

#### __in
Anything in this folder, actions, plugins are loaded/called at startup.    

This allows your plugin to add/remove actions.

#### __out
As with the above, actions listed in that folder are called when piger closes.

This allows for cleanup for example.

#### __temp

This is a special folder that is **not** parsed.
You can put your own files, code etc in that folder.


# Scripting your own commands.
## Lua
### Code

- am_say( what, elapse, fade ), display a message
- am_version, get the LUA version been used.
- am_getCommand( idx ), get a typed command, if the user selects something like "*google world*", then command #0 is "google" and command #1 is "world".
- am_getAction, get the selected action.
- am_getCommandCount, get the number of commands including the action.
- am_execute, execute a certain app/event/batch file.
- am_getstring, get a string that the user might have selected at the time of calling the action.
- am_getfile, get a file that the user might have selected at the time of calling the action.
- am_getfolder, get a folder that the user might have selected at the time of calling the action.
- am_geturl, get a url that the user might have selected at the time of calling the action.
- am_addAction, add an action to our current list of actions. (lost when the app restarts!)
- am_removeAction, remove an action from the list.
- am_getVersion, get the piger version number.
- am_findAction, find an action in our list of actions.

### Example

#### Calculator.lua

Launch the default calculator.

    am_say( "Run - calc ...", 400, 10 );
    am_execute( "%SystemRoot%\\system32\\calc.exe" );

#### Google.lua

Launch the explorer and navigate to the Google site, if one or more words are highlighted then those words will be searched.

	-- the browser to use
	browser = "iexplore"
    
	-- we need to know the number of arguments.
	sizeOf = am_getCommandCount();

	-- try and get the word that might be highlighted. 
	if sizeOf == 0  then
	  word = am_getstring();
	  if false == word then
	    am_say( "Starting Google.", 400, 10 );
	    am_execute( browser, "http://www.google.com/" );
	  else
	    am_say( "Starting Google for : " .. word , 400, 10 );
	    am_execute( browser, "http://www.google.com/search?hl=en&q=" .. word );
	  end
	else
	  query = ""
	  prettyQuery = "";
	  for count = 1, sizeOf, 1  do
	    -- the numbers are 0 based.
	    -- and we ignore the first one as it is the command itself
	    word = am_getCommand( count )
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

## Batch files

Windows batch files are just executed, they have no access to any of the plugins funtions.

Any arguments given are simply passed.

### Example

#### RootCommands.bat

Navigate to the root commands folder.

    START explorer /e,%appdata%\MyOddWeb\ActionMonitor\RootCommands\
