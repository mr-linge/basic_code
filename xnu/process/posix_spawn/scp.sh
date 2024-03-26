#!/usr/bin/expect

spawn scp -r -P 2222 ./main root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"

spawn scp -r -P 2222 ./target root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"
