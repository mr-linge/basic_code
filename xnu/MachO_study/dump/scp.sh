#!/usr/bin/expect

# spawn scp -r -P 2222 ./dump root@localhost:/Applications/
# expect "*root@iPhone:"
# send "alpine\n"
# expect "*#"

spawn scp -r -P 2222 ./libdump.dylib root@localhost:/Applications/
expect "*root@iPhone:"
send "alpine\n"
expect "*#"
