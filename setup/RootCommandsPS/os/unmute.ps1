# do the un-mute
Start-Service -DisplayName "Windows Audio"

# then give a message.
[void]($am.Say( "Audio un-mute", 400, 10 ))