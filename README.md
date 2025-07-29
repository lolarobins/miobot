# miobot
custom bot for the sputnik supporters discord to handle role selection and other fun lil things

## requirements

#### gcc/clang
works on darwin/linux/bsd/wsl, given that i have been using macos to work on this, the `CC` variable in the makefile should be swapped to gcc should you want to compile it without clang

#### concord library (https://github.com/Cogmasters/concord)
requires libcurl as a dependency  
installation:
```
git clone https://github.com/Cogmasters/concord
cd concord
make
sudo make install
```

#### defining the discord bot token
due to obvious safety reasons, the bot token is NOT stored in this repository. to set it, create a file in the parent directory named `bot_token` to insert it raw. it will be embedded upon building the program