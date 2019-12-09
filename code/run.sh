#/bin/bash
# protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto

# make clean
# make
set -e
make test_app
set +e

# Launch the GTStore Manager
init () {
  ./manager &
  MANAGER_PID=$!
  sleep 3

  # Launch couple GTStore Storage Nodes
  STORAGE_PIDS=()

  TO_INDEX="$(($1 - 1))"
  for i in `seq 0 $TO_INDEX`; do
    ./storage 0>/dev/null 1>/dev/null &
    STORAGE_PIDS+=($!)
    sleep 1
  done

}

cleanup () {
  for i in ${!STORAGE_PIDS[@]}; do
    kill -9 ${STORAGE_PIDS[$i]}
  done
  kill -9 $MANAGER_PID
  if pgrep manager; then pgrep manager | xargs kill; fi
  if pgrep storage; then pgrep storage | xargs kill; fi

}


benchmark () {
  init $1

  echo -n "" > benchmark.data
  TO_INDEX="$(($1 - 1))"
  for i in `seq 0 $TO_INDEX`; do
    ./test_app benchmark 1 &
    sleep 1
  done
  sleep "$(($1 + 5))"
  echo -n "NUM_PUT_REQUESTS for $1 nodes: "
  awk '{s+=$1} END {print s}' benchmark.data
  echo "==================="
  cleanup

}

# Launch the client testing app
# Usage: ./run.sh
init 5
./test_app basic_put_get
./test_app overriding_put_get
cleanup

init 2
./test_app put_get_faulty_node "${STORAGE_PIDS[0]}"
cleanup

init 2
./test_app put_get_faulty_node "${STORAGE_PIDS[1]}"
cleanup

init 10
./test_app data_partitioning_consistency "${STORAGE_PIDS[@]}"
cleanup

benchmark 1
benchmark 2
benchmark 3
benchmark 4

