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
  sleep 2

  # Launch couple GTStore Storage Nodes
  STORAGE_PIDS=()

  TO_INDEX="$(($1 - 1))"
  for i in `seq 0 $TO_INDEX`; do
    ./storage 0>/dev/null 1>/dev/null &
    STORAGE_PIDS+=($!)
  done

  sleep 2
}

cleanup () {
  for i in ${!STORAGE_PIDS[@]}; do
    kill ${STORAGE_PIDS[$i]}
  done
  kill $MANAGER_PID

}

# Launch the client testing app
# Usage: ./test_app <test> <client_id>
#init 5
#./test_app basic_put_get
#cleanup
#
#init 2
#./test_app put_get_faulty_node "${STORAGE_PIDS[0]}"
#cleanup
#
#init 2
#./test_app put_get_faulty_node "${STORAGE_PIDS[1]}"
#cleanup

init 5
./test_app data_partitioning "${STORAGE_PIDS[@]}"
cleanup

# ./test_app test_case2
# ./test_app test_case3

# Clean up - kill processes
