#!/usr/bin/expect

spawn scp -r -P 2222 ./test1 root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"

spawn scp -r -P 2222 ./test2 root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"

spawn scp -r -P 2222 ./test3 root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"
