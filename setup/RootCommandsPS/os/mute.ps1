# do the mute
Stop-Service -DisplayName "Windows Audio"

# then give a message.
[void]($am.Say( "Audio mute", 400, 10 ))