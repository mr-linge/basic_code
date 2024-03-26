#!/usr/bin/expect

spawn scp -r -P 2222 ./main root@localhost:/Applications
expect "*password:"
send "alpine\r"
expect "*#"
# interact
