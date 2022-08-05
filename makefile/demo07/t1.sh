path="./Payload"

t1(){
        if [ -d ${path} ]
        then
                mkdir "${path}/Frameworks"
        fi
}

t1

