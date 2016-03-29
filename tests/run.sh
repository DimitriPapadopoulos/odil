#! /bin/sh

set -e
set -u

configure() {
    cat > ${directory}/config  << EOF
HostTable BEGIN
remote = (REMOTE, localhost, 11112)
local = (LOCAL, localhost, 11113)
HostTable END

AETable BEGIN
REMOTE ${directory} RW (10, 1024mb) local
AETable END
EOF
}

add_data() {
dump2dcm --write-xfer-little /dev/stdin ${directory}/data.dcm <<EOF
(0008,0016) UI =RawDataStorage
(0008,0018) UI [2.25.95090344942250266709587559073467305647]
(0010,0010) PN [Doe^John]
(0010,0020) LO [DJ001]
EOF
storescu -aet LOCAL -aec REMOTE localhost 11112 ${directory}/data.dcm
}

start_scp() {
    dcmqrscp -c ${directory}/config 11112 &
    PID=$!
    sleep 1
}

stop_scp() {
    kill ${PID}
}

clean() {
    rm -rf ${directory}
}

directory=$(mktemp -d odil.XXX)
configure
start_scp
add_data

export ODIL_OWN_AET=LOCAL
export ODIL_PEER_HOST_NAME=127.0.0.1
export ODIL_PEER_PORT=11112
export ODIL_PEER_AET=REMOTE

export PATH=${PWD}/../build/tests/tools:$PATH
export PYTHONPATH=${PWD}/../build/wrappers

ctest --no-compress-output -T Test $@ || true
nosetests-2.7 ${PWD}/../tests/wrappers || true

stop_scp
clean

